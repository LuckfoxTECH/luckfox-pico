#!/bin/bash
set -e

ARG_COMMIT=$1
DIFF_SUBSET="scripts/.diff_*"
DIFF_DOC_ALL="scripts/.diff_all.txt"
DIFF_DOC_FIXED="scripts/.diff_fixed.txt"

function check_doc()
{
	local TOP_SEVERITY LANGUAGE=$1

	if [ "${LANGUAGE}" == "EN" ] ; then
		SVT_CRITIAL="critical"
		SVT_IMPORTANT="important"
		SVT_MODERATE="moderate"
		DOC=`git log ${ARG_COMMIT} -1 --name-only | sed -n "/_EN\.md/p"`
	else
		SVT_CRITIAL="紧急"
		SVT_IMPORTANT="重要"
		SVT_MODERATE="普通"
		DOC=`git log ${ARG_COMMIT} -1 --name-only | sed -n "/_CN\.md/p"`
	fi

	echo "Checking doc: ${DOC}"

	# check DOS encoding
	git show ${ARG_COMMIT} -1 ${DOC} | sed -n "/^+/p" > ${DIFF_DOC_ALL}
	git show ${ARG_COMMIT} -1 ${DOC} | sed -n "/^+/p" > ${DIFF_DOC_ALL}.dos
	dos2unix ${DIFF_DOC_ALL}.dos >/dev/null 2>&1
	CSUM1=`md5sum ${DIFF_DOC_ALL} | awk '{ print $1 }'`
	CSUM2=`md5sum ${DIFF_DOC_ALL}.dos | awk '{ print $1 }'`
	if [ "${CSUM1}" != "${CSUM2}" ]; then
		echo "ERROR: ${DOC} is DOS encoding. Fix it by: 'dos2unix ${DOC}'"
		exit 1
	fi

	TITLE=`sed -n "/^+## /p" ${DIFF_DOC_ALL} | tr -d " +#"`
	FILE=`sed -n "/^+| 20[0-9][0-9]-/p" ${DIFF_DOC_ALL} | tr -d " " | awk -F "|" '{ print $3 }'`
	COMMIT=`sed -n "/^+| 20[0-9][0-9]-/p" ${DIFF_DOC_ALL} | tr -d " " | awk -F "|" '{ print $4 }'`
	SEVERITY=`sed -n "/^+| 20[0-9][0-9]-/p" ${DIFF_DOC_ALL} | tr -d " " | awk -F "|" '{ print $5 }'`
	HORIZONTAL_LINE=`sed -n "/^+------$/p" ${DIFF_DOC_ALL}`
	# echo "### ${COMMIT}, ${SEVERITY}, ${TITLE}, ${FILE}"

	# check blank line after Heading 1
	HEADING_1=`sed -n '1p' ${DOC}`
	if sed -n '2p' ${DOC} | grep -q [a-z,A-Z] ; then
		echo "ERROR: ${DOC}: Should reserve blank line after '${HEADING_1}'"
		exit 1
	fi

	# check space
	if sed -n "/##/p" ${DOC} | grep -v '## [a-z,A-Z]' ; then
		echo "ERROR: ${DOC}: Should only 1 space between '#' and word"
		exit 1
	fi

	# check new content location
	if ! git show ${ARG_COMMIT} -1 ${DOC} | grep -q 'Release Note' ; then
		echo "ERROR: ${DOC}: Adding new content at the top but not bottom"
		exit 1
	fi

	# check TAB before index of 'New' body
	if grep -q $'\t[0-9]' ${DOC} ; then
		echo "ERROR: ${DOC}: Don't add TAB before index:"
		grep $'\t[0-9]' ${DOC}
		exit 1
	fi

	# check space after index of 'New' body
	SUM1=`grep '^[0-9]\.' ${DOC} | wc -l`
	SUM2=`grep '^[0-9]\.[[:blank:]]' ${DOC} | wc -l`
	if [ "$SUM1" != "$SUM2" ]; then
		echo "ERROR: ${DOC}: Please add space after index (e.g: '1. ' but not '1.'):"
		grep '^+[0-9]\.' ${DIFF_DOC_ALL}
		exit 1
	fi

	# check standalone file
	if ! echo ${FILE} | grep -Eq '\.bin|\.elf|\.img' ; then
		echo "ERROR: ${DOC}: '${FILE}' missing the file format suffix"
		exit 1
	fi
	if ! echo ${FILE} | grep -q { ; then
		if ! git log ${ARG_COMMIT} -1 --name-only | grep -q ${FILE}; then
			echo "ERROR: ${DOC}: '${FILE}' is not updated in this patch"
			exit 1
		fi
	fi

	# check title
	if [ "${TITLE}" != "${FILE}" ]; then
		echo "ERROR: ${DOC}: Title '${TITLE}' is not match with '${FILE}'"
		exit 1
	fi

	# check commit
	COMMIT=${COMMIT//#/ }
	for LIST in ${COMMIT}; do
		CMT=`echo ${LIST} | cut -d : -f 2`
		if ! git log ${ARG_COMMIT} -1 | grep -q ${CMT} ; then
			echo "ERROR: ${DOC}: '${CMT}' is not match in commit message"
			exit 1
		fi

		if ! echo ${FILE} | grep -q { ; then
			if echo ${FILE} | grep -Eq 'spl_|tpl_|bl31_|bl32_|tee_' ; then
				FILE_PATH=`find -name ${FILE}`
				if [ -z "${FILE_PATH}" ]; then
					echo "ERROR: ${DOC}: No ${FILE}"
					exit 1
				fi
				if ! strings ${FILE_PATH} | grep -q ${CMT} ; then
					echo "ERROR: ${DOC}: ${FILE} is not build from '${CMT}'"
					exit 1
				fi
			fi
		fi
	done

	# check severity
	if [ "${SEVERITY}" != "${SVT_CRITIAL}" -a "${SEVERITY}" != "${SVT_IMPORTANT}" -a "${SEVERITY}" != "${SVT_MODERATE}" ]; then
		echo "ERROR: ${DOC}: Unknown main severity: ${SEVERITY}"
		exit 1
	fi

	# check horizontal line
	if [ -z "${HORIZONTAL_LINE}" ]; then
		echo "ERROR: ${DOC}: No horizontal line '------' at the last of new content"
		exit 1
	fi

	# check 'Fixed' content
	if grep -q "^+### Fixed" ${DIFF_DOC_ALL} ; then
		awk -v RS='### Fixed' 'END{printf "%s", $0}' ${DIFF_DOC_ALL} > ${DIFF_DOC_FIXED}
		sed -i "/^$/d"    ${DIFF_DOC_FIXED}
		sed -i "/Index/d" ${DIFF_DOC_FIXED}
		sed -i "/---/d"   ${DIFF_DOC_FIXED}
		sed -i "/^+$/d"   ${DIFF_DOC_FIXED}

		while read LINE
		do
			EACH_SEVERITY=`echo "${LINE}" | awk -F "|" '{ print $3 }' | tr -d " "`
			if [ "${EACH_SEVERITY}" != "${SVT_CRITIAL}" -a "${EACH_SEVERITY}" != "${SVT_IMPORTANT}" -a "${EACH_SEVERITY}" != "${SVT_MODERATE}" ]; then
				echo "ERROR: ${DOC}: Unknown severity: ${EACH_SEVERITY}"
				exit 1
			fi

			# echo "## EACH: $EACH_SEVERITY"
			if [ -z "${TOP_SEVERITY}" ]; then
				TOP_SEVERITY="${EACH_SEVERITY}"
			elif [ "${TOP_SEVERITY}" == "${SVT_MODERATE}" ]; then
				if [ "${EACH_SEVERITY}" == "${SVT_CRITIAL}" -o "${EACH_SEVERITY}" == "${SVT_IMPORTANT}" ]; then
					TOP_SEVERITY="${EACH_SEVERITY}"
				fi
			elif [ "${TOP_SEVERITY}" == "${SVT_IMPORTANT}" ]; then
				if [ "${EACH_SEVERITY}" == "${SVT_CRITIAL}" ]; then
					TOP_SEVERITY="${EACH_SEVERITY}"
				fi
			fi
		done < ${DIFF_DOC_FIXED}

		if [ "${SEVERITY}" != "${TOP_SEVERITY}" ]; then
			echo "ERROR: ${DOC}: Main severity should be '${TOP_SEVERITY}'"
			exit 1
		fi
	fi
}

function check_docs()
{
	if git log ${ARG_COMMIT} -1 --name-only | grep -Eq '\.bin|\.elf' ; then
		DOC_CN=`git log ${ARG_COMMIT} -1 --name-only | sed -n "/_CN\.md/p"`
		DOC_EN=`git log ${ARG_COMMIT} -1 --name-only | sed -n "/_EN\.md/p"`
		if [ -z "${DOC_CN}" -o -z "${DOC_EN}" ]; then
			echo "ERROR: Should update CN and EN Release-Note when .bin/elf changed"
			exit 1
		fi

		if ! which dos2unix > /dev/null 2>&1 ; then
			echo "ERROR: No 'dos2unix'. Fix by: sudo apt-get install dos2unix"
			exit 1
		fi

		check_doc CN
		check_doc EN
	fi

	rm -f ${DIFF_SUBSET}
}

function pack_loader_image()
{
	for FILE in `ls ./RKBOOT/*MINIALL*.ini`
	do
		if [ "${FILE}" = "./RKBOOT/RK302AMINIALL.ini" -o \
			 "${FILE}" = "./RKBOOT/RK30BMINIALL.ini" -o \
			 "${FILE}" = "./RKBOOT/RK30MINIALL.ini" -o \
			 "${FILE}" = "./RKBOOT/RK310BMINIALL.ini" ]; then
			continue;
		fi

		if grep -q '^PATH=img/' ${FILE}; then
			continue;
		fi

		echo "Pack loader: ${FILE}"
		./tools/boot_merger ${FILE}
		rm -f *loader*.bin *download*.bin *idblock*.img
		echo
	done
}

function pack_trust_image()
{
	# Pack 32-bit trust
	for FILE in `ls ./RKTRUST/*TOS*.ini`
	do
		if ! test -s ${FILE}; then
			continue;
		elif ! grep -q 'TOS' ${FILE}; then
			continue;
		elif grep -q '^PATH=img/' ${FILE}; then
			continue;
		fi

		echo "Pack trust: ${FILE}"
		# Parse orignal path
		TOS=`sed -n "/TOS=/s/TOS=//p" ${FILE}|tr -d '\r'`
		TOS_TA=`sed -n "/TOSTA=/s/TOSTA=//p" ${FILE}|tr -d '\r'`

		# replace "./tools/rk_tools/" with "./" to compatible legacy ini content of rkdevelop branch
		TOS=$(echo ${TOS} | sed "s/tools\/rk_tools\//\.\//g")
		TOS_TA=$(echo ${TOS_TA} | sed "s/tools\/rk_tools\//\.\//g")

		if [ x${TOS_TA} != x -a x${TOS} != x ]; then
			./tools/loaderimage --pack --trustos ${TOS} ./trust.img 0x68400000
			./tools/loaderimage --pack --trustos ${TOS_TA} ./trust_with_ta.img 0x68400000
		elif [ ${TOS} ]; then
			./tools/loaderimage --pack --trustos ${TOS} ./trust.img 0x68400000
		elif [ ${TOS_TA} ]; then
			./tools/loaderimage --pack --trustos ${TOS_TA} ./trust.img 0x68400000
		else
			exit 1
		fi
		rm -f trust*.img
		echo
	done

	# Pack 64-bit trust
	for FILE in `ls ./RKTRUST/*TRUST*.ini`
	do
		if grep -q '^PATH=img/' ${FILE}; then
			continue;
		fi

		echo "Pack trust: ${FILE}"
		./tools/trust_merger ${FILE}
		rm -f trust*.img
		echo
	done
}

function check_dirty()
{
	for FILE in `find -name '*spl*.bin' -o -name '*tpl*.bin' -o -name '*usbplug*.bin' -o -name '*bl31*.elf' -o -name '*bl32*.bin'`; do
		echo "Checking clean: ${FILE}"
		if strings ${FILE} | grep '\-dirty ' ; then
			echo "ERROR: ${FILE} is dirty"
			exit 1
		fi
	done
}

function check_stripped()
{
	for FILE in `find -name '*bl31*.elf'`; do
		echo "Checking strip: ${FILE}"
		INFO=`file ${FILE}`
		if echo ${INFO} | grep -q "not stripped" ; then
			echo "ERROR: ${FILE} is not stripped"
			exit 1
		fi
	done
}

function check_mode()
{
	echo "Checking file mode..."
	if git whatchanged ${ARG_COMMIT} -1 --oneline | sed -n '/RKBOOT\//p; /RKTRUST\//p; /bin\//p; /doc\//p;' | awk '{ print $2 }' | grep -q 755 ; then
		git whatchanged ${ARG_COMMIT} -1 --oneline | sed -n '/RKBOOT\//p; /RKTRUST\//p; /bin\//p; /doc\//p;' | grep 755
		echo "ERROR: Set 644 file permission but not 755."
		exit 1
	fi
}

function finish()
{
	echo "OK, everything is nice."
	echo
}

check_mode
check_docs
check_dirty
check_stripped
pack_loader_image
pack_trust_image
finish
