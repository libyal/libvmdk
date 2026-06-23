#!/bin/sh
# Script that synchronizes the local test data

TESTS_INPUT_DIRECTORY="tests/input"
TEST_SET="public"
TEST_FILES="ext2.vmdk"

mkdir -p "${TESTS_INPUT_DIRECTORY}/${TEST_SET}"

for TEST_FILE in ${TEST_FILES}
do
	URL="https://raw.githubusercontent.com/log2timeline/dfvfs/refs/heads/main/test_data/${TEST_FILE}"

	curl -L -o "${TESTS_INPUT_DIRECTORY}/${TEST_SET}/${TEST_FILE}" ${URL}
done
