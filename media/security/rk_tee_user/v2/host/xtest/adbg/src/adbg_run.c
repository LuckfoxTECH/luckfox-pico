// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 */

/*************************************************************************
 * 1. Includes
 ************************************************************************/
#include "adbg_int.h"

/*************************************************************************
 * 2. Definition of external constants and variables
 ************************************************************************/
#ifndef TAILQ_CONCAT
#define	TAILQ_CONCAT(head1, head2, field) do { \
	if (!TAILQ_EMPTY(head2)) { \
	*(head1)->tqh_last = (head2)->tqh_first; \
	(head2)->tqh_first->field.tqe_prev = (head1)->tqh_last; \
	(head1)->tqh_last = (head2)->tqh_last; \
	TAILQ_INIT((head2)); \
	} \
} while (/*CONSTCOND*/0)
#endif

/*************************************************************************
 * 3. File scope types, constants and variables
 ************************************************************************/

TAILQ_HEAD(ADBG_CaseHead, ADBG_Case);
typedef struct ADBG_CaseHead ADBG_CaseHead_t;

typedef struct ADBG_Runner {
	ADBG_Result_t Result;
	const ADBG_Suite_Definition_t *Suite_p;

	ADBG_CaseHead_t CasesList;
} ADBG_Runner_t;

/*************************************************************************
 * 4. Declaration of file local functions
 ************************************************************************/

static int ADBG_RunSuite(ADBG_Runner_t *Runner_p, int argc, char *argv[]);


/*************************************************************************
 * 5. Definition of external functions
 ************************************************************************/
int Do_ADBG_RunSuite(
	const ADBG_Suite_Definition_t *Suite_p,
	int argc,
	char *argv[]
	)
{
	ADBG_Runner_t *Runner_p = NULL;

	Runner_p = calloc(1, sizeof(*Runner_p));
	if (Runner_p == NULL) {
		Do_ADBG_Log("calloc failed for Suite %s!",
			    Suite_p->SuiteID_p);
		return -1;
	}
	TAILQ_INIT(&Runner_p->CasesList);
	Runner_p->Suite_p = Suite_p;

	int ret = ADBG_RunSuite(Runner_p, argc, argv);
	free(Runner_p);
	return ret;
}

int Do_ADBG_AppendToSuite(
	ADBG_Suite_Definition_t *Dest_p,
	ADBG_Suite_Definition_t *Source_p
	)
{
	char *p = NULL;
	size_t size = 0;

	/* Append name of 'Source_p' to name of 'Dest_p' */
	size = strlen(Source_p->SuiteID_p);
	if (Dest_p->SuiteID_p) {
		size += strlen(Dest_p->SuiteID_p);
		size += 1; /* '+' */
	}
	size += 1; /* '\0' */
	p = malloc(size);
	if (!p) {
		fprintf(stderr, "malloc failed\n");
		return -1;
	}
	if (Dest_p->SuiteID_p)
		snprintf(p, size, "%s+%s", Dest_p->SuiteID_p,
			 Source_p->SuiteID_p);
	else
		snprintf(p, size, "%s", Source_p->SuiteID_p);
	free((void *)Dest_p->SuiteID_p);
	Dest_p->SuiteID_p = p;

	TAILQ_CONCAT(&Dest_p->cases, &Source_p->cases, link);
	return 0;
}

/*************************************************************************
 * 6. Definitions of internal functions
 ************************************************************************/
static int ADBG_RunSuite(
	ADBG_Runner_t *Runner_p,
	int argc,
	char *argv[]
	)
{
	ADBG_Case_t *Case_p = NULL;
	size_t NumSkippedTestCases = 0;
	int failed_test = 0;
	struct adbg_case_def *case_def = NULL;

	Do_ADBG_Log("######################################################");
	Do_ADBG_Log("#");
	Do_ADBG_Log("# %s", Runner_p->Suite_p->SuiteID_p);
	Do_ADBG_Log("#");
	Do_ADBG_Log("######################################################");

	TAILQ_FOREACH(case_def, &Runner_p->Suite_p->cases, link) {
		/* Default match if by default all tests are to be run */
		bool HaveMatch = !argc || !strcmp(argv[0], "-x");
		int i = 0;

		for (i = 0; i < argc; i++) {
			if (!strcmp(argv[i], "-x")) {
				i++;
				if (i >= argc) {
					Do_ADBG_Log("Error: -x <test-id>");
					return 1;
				}
				if (strstr(case_def->TestID_p, argv[i]))
					HaveMatch = false;
			} else {
				if (strstr(case_def->TestID_p, argv[i]))
					HaveMatch = true;
			}
		}
		if (!HaveMatch) {
			NumSkippedTestCases++;
			continue;
		}

		Case_p = ADBG_Case_New(case_def);
		if (Case_p == NULL) {
			Do_ADBG_Log("HEAP_ALLOC failed for Case %s!",
				    case_def->TestID_p);
			Runner_p->Result.AbortTestSuite = 1;
			break;
		}

		TAILQ_INSERT_TAIL(&Runner_p->CasesList, Case_p, Link);

		/* Start the parent test case */
		Do_ADBG_BeginSubCase(Case_p, "%s", case_def->Title_p);

		case_def->Run_fp(Case_p);

		/* End abondoned subcases */
		while (Case_p->CurrentSubCase_p != Case_p->FirstSubCase_p)
			Do_ADBG_EndSubCase(Case_p, NULL);

		/* End the parent test case */
		Do_ADBG_EndSubCase(Case_p, "%s", case_def->Title_p);

		/* Sum up the errors */
		Runner_p->Result.NumTests += Case_p->Result.NumTests +
					     Case_p->Result.NumSubTests;
		Runner_p->Result.NumFailedTests +=
			Case_p->Result.NumFailedTests +
			Case_p->Result.
			NumFailedSubTests;
		Runner_p->Result.NumSubCases++;
		if (Case_p->Result.NumFailedTests +
		    Case_p->Result.NumFailedSubTests > 0)
			Runner_p->Result.NumFailedSubCases++;

		Runner_p->Result.AbortTestSuite = Case_p->Result.AbortTestSuite;

		if (Runner_p->Result.AbortTestSuite) {
			Do_ADBG_Log("Test suite aborted by %s!",
				    case_def->TestID_p);
			break;
		}
	}

	Do_ADBG_Log("+-----------------------------------------------------");
	if (argc > 0) {
		bool test_exclusion = false;
		int i = 0;

		for (i = 0; i < argc; i++) {
			if (!strcmp(argv[i], "-x")) {
				test_exclusion = true;
				continue;
			}
			Do_ADBG_Log(
				"Result of testsuite %s filtered by \"%s%s\":",
				Runner_p->Suite_p->SuiteID_p,
				test_exclusion ? "-x " : "", argv[i]);
			test_exclusion = false;
		}
	} else {
		Do_ADBG_Log("Result of testsuite %s:",
			    Runner_p->Suite_p->SuiteID_p);
	}

	TAILQ_FOREACH(Case_p, &Runner_p->CasesList, Link) {
		ADBG_SubCase_Iterator_t Iterator;
		ADBG_SubCase_t *SubCase_p;

		ADBG_Case_IterateSubCase(Case_p, &Iterator);
		while ((SubCase_p = ADBG_Case_NextSubCase(&Iterator)) != NULL) {
			if (SubCase_p->Result.NumFailedTests +
			    SubCase_p->Result.NumFailedSubTests > 0) {
				if (SubCase_p->Result.FirstFailedFile_p !=
				    NULL) {
					Do_ADBG_Log(
						"%s FAILED first error at %s:%d",
						SubCase_p->TestID_p,
						SubCase_p->
							Result.FirstFailedFile_p,
						SubCase_p->
							Result.FirstFailedRow);
				} else {
					Do_ADBG_Log("%s FAILED",
						    SubCase_p->TestID_p);
				}
			} else if (ADBG_Case_SubCaseIsMain(Case_p, SubCase_p)) {
				/* A level one test case is displayed
					if successfull too */
				Do_ADBG_Log("%s OK", SubCase_p->TestID_p);
			}
		}
	}


	Do_ADBG_Log("+-----------------------------------------------------");
	if (Runner_p->Result.AbortTestSuite)
		Do_ADBG_Log("Test suite was ABORTED");

	Do_ADBG_Log("%d subtest%s of which %d failed",
		    Runner_p->Result.NumTests,
		    Runner_p->Result.NumTests != 1 ? "s" : "",
		    Runner_p->Result.NumFailedTests);
	Do_ADBG_Log("%d test case%s of which %d failed",
		    Runner_p->Result.NumSubCases,
		    Runner_p->Result.NumSubCases != 1 ? "s" : "",
		    Runner_p->Result.NumFailedSubCases);
	Do_ADBG_Log("%zu test case%s skipped",
		    NumSkippedTestCases,
		    NumSkippedTestCases != 1 ? "s were" : " was");

	failed_test = Runner_p->Result.NumFailedSubCases;

	while (true) {
		Case_p = TAILQ_FIRST(&Runner_p->CasesList);
		if (Case_p == NULL)
			break;
		TAILQ_REMOVE(&Runner_p->CasesList, Case_p, Link);
		ADBG_Case_Delete(Case_p);
	}
	return failed_test;
}
