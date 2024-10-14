#!/bin/bash
if [[ $REPO_REMOTE == rk* ]]; then
    REMOTE_DIFF=`git log $REPO_LREV..HEAD`
    LOCAL_DIFF=`git diff`
    if [ -n "$REMOTE_DIFF" ]; then
        mkdir -p $STUB_PATCH_PATH/$REPO_PATH/
        git format-patch $REPO_LREV..HEAD -o $STUB_PATCH_PATH/$REPO_PATH

        echo "remote url:" >> $STUB_PATCH_PATH/$REPO_PATH/git-merge-base.txt
        REMOTE_URL=`git remote -v`
        echo "$REMOTE_URL" >> $STUB_PATCH_PATH/$REPO_PATH/git-merge-base.txt
        echo "remote branch:" >> $STUB_PATCH_PATH/$REPO_PATH/git-merge-base.txt
        git branch --contains $REPO_LREV -r >> $STUB_PATCH_PATH/$REPO_PATH/git-merge-base.txt
        git merge-base HEAD $REPO_LREV | xargs git show -s >> $STUB_PATCH_PATH/$REPO_PATH/git-merge-base.txt
    fi

    if [ -n "$LOCAL_DIFF" ]; then
        mkdir -p $STUB_PATCH_PATH/$REPO_PATH/
        git diff --binary > $STUB_PATCH_PATH/$REPO_PATH/local_diff.patch
    fi
    if [ -n "$REMOTE_DIFF" -o -n "$LOCAL_DIFF" ]; then
        echo Generate patch for "$REPO_PATH" done
    fi
fi
