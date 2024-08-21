// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 */

/*************************************************************************
* 1. Includes
*************************************************************************/
#include "adbg_int.h"

/*************************************************************************
* 2. Definition of external constants and variables
*************************************************************************/

/*************************************************************************
* 3. File scope types, constants and variables
*************************************************************************/

/*************************************************************************
* 4. Declaration of file local functions
*************************************************************************/

/*
 * Deletes a subcase. Don't call this function before the
 * subcase is removed from list.
 */
static void ADBG_SubCase_Delete(ADBG_SubCase_t *SubCase);

static ADBG_SubCase_t *ADBG_Case_CreateSubCase(ADBG_Case_t *Case_p,
					       const char *const Title_p);

static ADBG_SubCase_t *ADBG_Case_GetParentSubCase(ADBG_Case_t *Case_p,
						  ADBG_SubCase_t *SubCase_p);

static const char *ADBG_Case_GetTestID(ADBG_Case_t *Case_p);

/*************************************************************************
* 5. Definition of external functions
*************************************************************************/
ADBG_Case_t *ADBG_Case_New(const struct adbg_case_def *case_def)
{
	ADBG_Case_t *Case_p = NULL;

	Case_p = calloc(1, sizeof(*Case_p));
	if (Case_p)
		Case_p->case_def = case_def;

	return Case_p;
}

void ADBG_Case_Delete(ADBG_Case_t *Case_p)
{
	ADBG_SubCase_Delete(Case_p->FirstSubCase_p);
	free(Case_p);
}

bool ADBG_Case_SubCaseIsMain(
	const ADBG_Case_t *const Case_p,
	const ADBG_SubCase_t *const SubCase_p
	)
{
	IDENTIFIER_NOT_USED(Case_p)
	return SubCase_p->Parent_p == NULL;
}


void ADBG_Case_IterateSubCase(
	ADBG_Case_t *Case_p,
	ADBG_SubCase_Iterator_t *Iterator_p
	)
{
	Iterator_p->Case_p = Case_p;
	Iterator_p->CurrentSubCase_p = NULL;
}

ADBG_SubCase_t *ADBG_Case_NextSubCase(
	ADBG_SubCase_Iterator_t *Iterator_p
	)
{
	ADBG_Case_t *Case_p = Iterator_p->Case_p;
	ADBG_SubCase_t *SubCase_p = Iterator_p->CurrentSubCase_p;


	/*
	 * Traverse the subcases depth first, that is:
	 * 1.1.1.1
	 * 1.1.1.2
	 * 1.1.1
	 * 1.1.2.1
	 * 1.1.2
	 * 1.1
	 * 1.2.1
	 * 1.2
	 * 1
	 */
	if (SubCase_p == NULL) {
		/* Find the first leaf */
		SubCase_p = Case_p->FirstSubCase_p;
		if (SubCase_p == NULL)
			goto CleanupReturn;

		while (!TAILQ_EMPTY(&SubCase_p->SubCasesList))
			SubCase_p = TAILQ_FIRST(&SubCase_p->SubCasesList);
		goto CleanupReturn;
	}

	/*
	 * Look for the next leaf belonging to the parent
	 */

	if (SubCase_p->Parent_p == NULL) {
		/* If parent is NULL this is the top
			subcase and we're done */
		SubCase_p = NULL;
		goto CleanupReturn;
	}

	if (TAILQ_NEXT(SubCase_p, Link) == NULL) {
		/* If this is the last subcase of the
			parent move up to parent */
		SubCase_p = SubCase_p->Parent_p;
		goto CleanupReturn;
	}

	/*
	 * Find next leaf
	 */
	SubCase_p = TAILQ_NEXT(SubCase_p, Link);
	while (!TAILQ_EMPTY(&SubCase_p->SubCasesList))
		SubCase_p = TAILQ_FIRST(&SubCase_p->SubCasesList);

CleanupReturn:
	Iterator_p->CurrentSubCase_p = SubCase_p;
	return SubCase_p;
}

void Do_ADBG_BeginSubCase(
	ADBG_Case_t *const Case_p,
	const char *const FormatTitle_p, ...
	)
{
	ADBG_SubCase_t *SubCase_p = NULL;

	if (Case_p == NULL) {
		Do_ADBG_Log("Do_ADBG_BeginSubCase: NULL Case_p!");
		return;
	}

	if (FormatTitle_p == NULL) {
		Do_ADBG_Log("Do_ADBG_BeginSubCase: NULL FormatTitle_p!");
		return;
	}

	va_list ArgList;
	char Title[80] = { };

	va_start(ArgList, FormatTitle_p);
	vsnprintf(Title, sizeof(Title), FormatTitle_p, ArgList);
	va_end(ArgList);

	SubCase_p = ADBG_Case_CreateSubCase(Case_p, Title);

	if (SubCase_p == NULL) {
		Do_ADBG_Log("Do_ADBG_BeginSubCase: HEAP_ALLOC failed");
		return;
	}


	if (ADBG_Case_SubCaseIsMain(Case_p, SubCase_p)) {
		/* Main SubCase */
		Do_ADBG_Log(" ");
		Do_ADBG_Log("* %s %s", SubCase_p->TestID_p, SubCase_p->Title_p);
	} else {
		Do_ADBG_Log("o %s %s", SubCase_p->TestID_p, SubCase_p->Title_p);
	}
}

void Do_ADBG_EndSubCase(
	ADBG_Case_t *const Case_p,
	const char *const FormatTitle_p, ...
	)
{
	va_list ArgList;
	char Title[80] = { };
	ADBG_SubCase_t *SubCase_p = NULL;

	if (Case_p == NULL) {
		Do_ADBG_Log("Do_ADBG_EndSubCase: NULL Case_p!");
		return;
	}

	if (FormatTitle_p == NULL) {
		strcpy(Title, "NULL");
	} else {
		va_start(ArgList, FormatTitle_p);
		vsnprintf(Title, sizeof(Title), FormatTitle_p, ArgList);
		va_end(ArgList);
	}


	SubCase_p = Case_p->CurrentSubCase_p;

	if (SubCase_p == NULL) {
		Do_ADBG_Log("Do_ADBG_EndSubCase: "
			    "Have no active SubCase, bailing out for title \"%s\"",
			    Title);
		return;
	}

	if (FormatTitle_p != NULL && strcmp(SubCase_p->Title_p, Title) != 0) {
		Do_ADBG_Log("Do_ADBG_EndSubCase: "
			    "Active SubCase \"%s\" doesn't match supplied title \"%s\"",
			    SubCase_p->Title_p, Title);
		return;
	}

	if (ADBG_Case_SubCaseIsMain(Case_p, SubCase_p)) {
		if (FormatTitle_p == NULL) {
			/* To end the main subcase we require
				a matching title */
			Do_ADBG_Log("Do_ADBG_EndSubCase: "
				    "The main SubCase \"%s\" doesn't match supplied title \"%s\"",
				    SubCase_p->Title_p, Title);
			return;
		}
		/*
		 * Ending the main subcase
		 * make a complete copy of the aggregated result.
		 */
		Case_p->Result = SubCase_p->Result;
	} else {
		/*
		 * Ending a subcase,
		 * Aggregate results to parent.
		 */
		ADBG_SubCase_t *Parent_p = SubCase_p->Parent_p;

		Parent_p->Result.NumSubTests += SubCase_p->Result.NumTests +
						SubCase_p->Result.NumSubTests;
		Parent_p->Result.NumFailedSubTests +=
			SubCase_p->Result.NumFailedTests +
			SubCase_p->Result.
			NumFailedSubTests;
		Parent_p->Result.AbortTestSuite =
			SubCase_p->Result.AbortTestSuite;
		if (SubCase_p->Result.NumTests > 0 ||
		    SubCase_p->Result.NumSubTests > 0)
			Parent_p->Result.NumFailedSubCases++;
	}

	/* Print a summary of the subcase result */
	if (SubCase_p->Result.NumFailedTests > 0 ||
	    SubCase_p->Result.NumFailedSubTests > 0) {
		Do_ADBG_Log("  %s FAILED", SubCase_p->TestID_p);
	} else {
		Do_ADBG_Log("  %s OK", SubCase_p->TestID_p);
	}

	/* Update current subcase to be the parent of this subcase */
	Case_p->CurrentSubCase_p =
		ADBG_Case_GetParentSubCase(Case_p, SubCase_p);
}

/*************************************************************************
* 6. Definition of internal functions
*************************************************************************/
static ADBG_SubCase_t *ADBG_Case_CreateSubCase(
	ADBG_Case_t *Case_p,
	const char *const Title_p
	)
{
	ADBG_SubCase_t *SubCase_p = NULL;

	SubCase_p = calloc(1, sizeof(*SubCase_p));
	if (SubCase_p == NULL)
		goto ErrorReturn;

	TAILQ_INIT(&SubCase_p->SubCasesList);

	SubCase_p->Title_p = strdup(Title_p);
	if (SubCase_p->Title_p == NULL)
		goto ErrorReturn;

	/* Set parent pointer needed "early" below. */
	SubCase_p->Parent_p = Case_p->CurrentSubCase_p;

	if (SubCase_p->Parent_p == NULL) {
		/* Main SubCase */
		SubCase_p->TestID_p = strdup(ADBG_Case_GetTestID(Case_p));
		if (SubCase_p->TestID_p == NULL)
			goto ErrorReturn;

		Case_p->FirstSubCase_p = SubCase_p;
	} else {
		ADBG_SubCase_t *Parent_p = SubCase_p->Parent_p;
		char PrefixTitle[80] = { };

		/* Update parent SubCase */
		Parent_p->Result.NumSubCases++;
		snprintf(PrefixTitle, sizeof(PrefixTitle), "%s.%d",
			 Parent_p->TestID_p, Parent_p->Result.NumSubCases);
		SubCase_p->TestID_p = strdup(PrefixTitle);
		if (SubCase_p->TestID_p == NULL)
			goto ErrorReturn;

		TAILQ_INSERT_TAIL(&Parent_p->SubCasesList, SubCase_p, Link);
	}

	Case_p->CurrentSubCase_p = SubCase_p;
	return SubCase_p;

ErrorReturn:
	ADBG_SubCase_Delete(SubCase_p);
	return NULL;
}

static void ADBG_SubCase_Delete(
	ADBG_SubCase_t *SubCase_p
	)
{
	if (SubCase_p != NULL) {
		/*
		 * Note that Util_ListDestroy() checks
		 * if SubCase_p->SubCasesList_p
		 * is NULL.
		 */
		while (true) {
			ADBG_SubCase_t *s =
				TAILQ_FIRST(&SubCase_p->SubCasesList);

			if (s == NULL)
				break;

			TAILQ_REMOVE(&SubCase_p->SubCasesList, s, Link);
			ADBG_SubCase_Delete(s);
		}
		free(SubCase_p->TestID_p);
		free(SubCase_p->Title_p);
		free(SubCase_p);
	}
}

ADBG_SubCase_t *ADBG_Case_GetParentSubCase(
	ADBG_Case_t *Case_p,
	ADBG_SubCase_t *SubCase_p
	)
{
	IDENTIFIER_NOT_USED(Case_p)
	IDENTIFIER_NOT_USED(SubCase_p)
	return SubCase_p->Parent_p;
}

static const char *ADBG_Case_GetTestID(ADBG_Case_t *Case_p)
{
	IDENTIFIER_NOT_USED(Case_p)

	return Case_p->case_def->TestID_p;
}
