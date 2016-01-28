#!/usr/bin/env python
#
# Python-bindings read testing program
#
# Copyright (C) 2009-2016, Joachim Metz <joachim.metz@gmail.com>
#
# Refer to AUTHORS for acknowledgements.
#
# This software is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this software.  If not, see <http://www.gnu.org/licenses/>.
#

from __future__ import print_function
import argparse
import os
import sys

import pyvmdk


def get_whence_string(whence):
  """Retrieves a human readable string representation of the whence."""
  if whence == os.SEEK_CUR:
    whence_string = "SEEK_CUR"
  elif whence == os.SEEK_END:
    whence_string = "SEEK_END"
  elif whence == os.SEEK_SET:
    whence_string = "SEEK_SET"
  else:
    whence_string = "UNKNOWN"
  return whence_string


def pyvmdk_test_seek_offset_and_read_buffer(
    vmdk_handle, input_offset, input_whence, input_size, expected_offset,
    expected_size):

  print(
      ("Testing reading buffer at offset: {0:d}, whence: {1:s} and "
       "size: {2:d}\t").format(
          input_offset, get_whence_string(input_whence), input_size),
      end="")

  error_string = ""
  result = True
  try:
    vmdk_handle.seek(input_offset, input_whence)

    result_offset = vmdk_handle.get_offset()
    if expected_offset != result_offset:
      result = False

    if result:
      result_size = 0
      while input_size > 0:
        read_size = 4096
        if input_size < read_size:
          read_size = input_size

        data = vmdk_handle.read(size=read_size)
        data_size = len(data)

        input_size -= data_size
        result_size += data_size

        if data_size != read_size:
          break

      if result_size != expected_size:
        error_string = "Unexpected read count: {0:d}".format(result_size)
        result = False

  except Exception as exception:
    print(str(exception))
    if expected_offset != -1:
      result = False

  if not result:
    print("(FAIL)")
  else:
    print("(PASS)")

  if error_string:
    print(error_string)
  return result


def pyvmdk_test_read_buffer_at_offset(
    vmdk_handle, input_offset, input_size, expected_offset, expected_size):

  print("Testing reading buffer at offset: {0:d} and size: {1:d}\t".format(
      input_offset, input_size), end="")

  error_string = ""
  result = True
  try:
    result_size = 0
    while input_size > 0:
      read_size = 4096
      if input_size < read_size:
        read_size = input_size

      data = vmdk_handle.read_buffer_at_offset(read_size, input_offset)
      data_size = len(data)

      input_offset += data_size
      input_size -= data_size
      result_size += data_size

      if data_size != read_size:
        break

    if input_offset != expected_offset:
      error_string = "Unexpected offset: {0:d}".format(input_offset)
      result = False

    elif result_size != expected_size:
      error_string = "Unexpected read count: {0:d}".format(result_size)
      result = False

  except Exception as exception:
    print(str(exception))
    if expected_offset != -1:
      result = False

  if not result:
    print("(FAIL)")
  else:
    print("(PASS)")

  if error_string:
    print(error_string)
  return result


def pyvmdk_test_read(vmdk_handle):
  media_size = vmdk_handle.media_size

  # Case 0: test full read

  # Test: offset: 0 size: <media_size>
  # Expected result: offset: 0 size: <media_size>
  if not pyvmdk_test_seek_offset_and_read_buffer(
      vmdk_handle, 0, os.SEEK_SET, media_size, 0, media_size):
    return False

  # Test: offset: 0 size: <media_size>
  # Expected result: offset: 0 size: <media_size>
  if not pyvmdk_test_seek_offset_and_read_buffer(
      vmdk_handle, 0, os.SEEK_SET, media_size, 0, media_size):
    return False

  # Case 1: test buffer at offset read

  # Test: offset: <media_size / 7> size: <media_size / 2>
  # Expected result: offset: <media_size / 7> size: <media_size / 2>
  if not pyvmdk_test_seek_offset_and_read_buffer(
      vmdk_handle, media_size / 7, os.SEEK_SET, media_size / 2,
      media_size / 7, media_size / 2):
    return False

  # Test: offset: <media_size / 7> size: <media_size / 2>
  # Expected result: offset: <media_size / 7> size: <media_size / 2>
  if not pyvmdk_test_seek_offset_and_read_buffer(
      vmdk_handle, media_size / 7, os.SEEK_SET, media_size / 2,
      media_size / 7, media_size / 2):
    return False

  # Case 2: test read beyond media size

  if media_size < 1024:
    # Test: offset: <media_size - 1024> size: 4096
    # Expected result: offset: -1 size: <undetermined>
    if not pyvmdk_test_seek_offset_and_read_buffer(
        vmdk_handle, media_size - 1024, os.SEEK_SET, 4096, -1, -1):
      return False

    # Test: offset: <media_size - 1024> size: 4096
    # Expected result: offset: -1 size: <undetermined>
    if not pyvmdk_test_seek_offset_and_read_buffer(
        vmdk_handle, media_size - 1024, os.SEEK_SET, 4096, -1, -1):
      return False

  else:
    # Test: offset: <media_size - 1024> size: 4096
    # Expected result: offset: <media_size - 1024> size: 1024
    if not pyvmdk_test_seek_offset_and_read_buffer(
        vmdk_handle, media_size - 1024, os.SEEK_SET, 4096,
        media_size - 1024, 1024):
      return False

    # Test: offset: <media_size - 1024> size: 4096
    # Expected result: offset: <media_size - 1024> size: 1024
    if not pyvmdk_test_seek_offset_and_read_buffer(
        vmdk_handle, media_size - 1024, os.SEEK_SET, 4096,
        media_size - 1024, 1024):
      return False

  # Case 3: test buffer at offset read

  # Test: offset: <media_size / 7> size: <media_size / 2>
  # Expected result: offset: < ( media_size / 7 ) + ( media_size / 2 ) > size: <media_size / 2>
  if not pyvmdk_test_read_buffer_at_offset(
      vmdk_handle, media_size / 7, media_size / 2,
      (media_size / 7) + (media_size / 2), media_size / 2):
    return False

  # Test: offset: <media_size / 7> size: <media_size / 2>
  # Expected result: offset: < ( media_size / 7 ) + ( media_size / 2 ) > size: <media_size / 2>
  if not pyvmdk_test_read_buffer_at_offset(
      vmdk_handle, media_size / 7, media_size / 2,
      (media_size / 7) + (media_size / 2), media_size / 2):
    return False

  return True


def pyvmdk_test_read_file(filename):
  vmdk_handle = pyvmdk.handle()

  vmdk_handle.open(filename, "r")
  vmdk_handle.open_extent_data_files()
  result = pyvmdk_test_read(vmdk_handle)
  vmdk_handle.close()

  return result


def pyvmdk_test_read_file_object(filename):
  file_object = open(filename, "rb")
  vmdk_handle = pyvmdk.handle()

  vmdk_handle.open_file_object(file_object, "r")

  base_directory = os.path.dirname(filename)

  extent_data_files = []
  for extent_descriptor in vmdk_handle.extent_descriptors:
    extent_data_filename = extent_descriptor.filename

    _, path_separator, filename = extent_data_filename.rpartition(u"/")
    if not path_separator:
      _, path_separator, filename = extent_data_filename.rpartition(u"\\")

    if not path_separator:
      filename = extent_data_filename

    extent_data_file_path = os.path.join(base_directory, filename)

    if not os.path.exists(extent_data_file_path):
      break

    extent_data_files.append(extent_data_file_path)

  if len(extent_data_files) != vmdk_handle.number_of_extents:
    print("Unable to locate all extent data files.\n")
    return False

  file_objects = []
  for extent_data_file_path in extent_data_files:
    file_object = open(extent_data_file_path, "rb")
    file_objects.append(file_object)

  vmdk_handle.open_extent_data_files_file_objects(file_objects)
  result = pyvmdk_test_read(vmdk_handle)
  vmdk_handle.close()

  return result


def pyvmdk_test_read_file_no_open(filename):
  print("Testing read of offset without open:\t", end="")

  vmdk_handle = pyvmdk.handle()

  error_string = ""
  result = False
  try:
    vmdk_handle.read(size=4096)
  except Exception as exception:
    error_string = str(exception)
    result = True

  if not result:
    print("(FAIL)")
  else:
    print("(PASS)")

  if error_string:
    print(error_string)
  return result


def main():
  args_parser = argparse.ArgumentParser(description=(
      "Tests read."))

  args_parser.add_argument(
      "source", nargs="?", action="store", metavar="FILENAME",
      default=None, help="The source filename.")

  options = args_parser.parse_args()

  if not options.source:
    print("Source value is missing.")
    print("")
    args_parser.print_help()
    print("")
    return False

  if not pyvmdk_test_read_file(options.source):
    return False

  if not pyvmdk_test_read_file_object(options.source):
    return False

  if not pyvmdk_test_read_file_no_open(options.source):
    return False

  return True


if __name__ == "__main__":
  if not main():
    sys.exit(1)
  else:
    sys.exit(0)

