#include <stdlib.h>
#include <stdio.h>
#include <libvmdk.h>
#include <string>

#define LIBVMDK_MAX_ERROR_LENGTH 1024

typedef struct _go_bytebuffer {
  void *buffer;
  size_t size;
} go_bytebuffer;

namespace LibVMDK {
// --------

class VMDKBase {

protected:
  libvmdk_handle_t *handle;
  libvmdk_error_t *errorH;
  char *errorString;

  void saveError() {
    if (this->errorH != NULL) {
        if (this->errorString != NULL) {
          free(this->errorString);
          this->errorString = NULL;
        }

        this->errorString = (char *)calloc(LIBVMDK_MAX_ERROR_LENGTH, sizeof(char));
        if (this->errorString != NULL) {
          libvmdk_error_sprint(this->errorH, this->errorString, LIBVMDK_MAX_ERROR_LENGTH / sizeof(char));
        } else {
          // malloc failure, eh...
        }

        libvmdk_error_free(&this->errorH);
        this->errorH = NULL;
    }
  }

  int handleError(int ret) {
    this->saveError();
    return ret;
  }

  long handleError(long ret) {
    this->saveError();
    return ret;
  }

public:
  VMDKBase() {
    this->handle = NULL; this->errorH = NULL; this->errorString = NULL;
  }

  ~VMDKBase() {
    this->handleError(1);
    if (this->errorString != NULL) {
      free(this->errorString);
      this->errorString = NULL;
    }
  }

  const char *error() {
    return this->errorString;
  }
};

class VMDKExtent : public VMDKBase {

protected:
  libvmdk_extent_descriptor_t *extent;

public:

  // Constructor
  VMDKExtent(libvmdk_handle_t *handle, libvmdk_extent_descriptor_t *extent, char *errorString = NULL) : VMDKBase() {
    if (errorString == NULL) {
      this->handle = handle;
      this->extent = extent;
    } else {
      this->handle = NULL; this->extent = NULL;
      this->errorString = errorString;
    }
  }

  // Destructor
  ~VMDKExtent() {
    if (this->extent != NULL) {
      libvmdk_extent_descriptor_free(&this->extent, &this->errorH);
    }
  }

  std::string getFilename() {
    size_t size = 0;
    char *filename = NULL;
    int ret;

    ret = libvmdk_extent_descriptor_get_utf8_filename_size(this->extent, &size, &this->errorH);
    if (ret != -1) {
      filename = (char *)calloc(size, sizeof(uint8_t));
      ret = libvmdk_extent_descriptor_get_utf8_filename(this->extent, (uint8_t *)filename, size, &this->errorH);
      if (ret != -1) {
        std::string str((const char *)filename);
        free(filename);
        return str;
      }
     }
    return std::string("");
  }
};

class VMDK : public VMDKBase {

public:
  // Constructor
  VMDK() : VMDKBase() {
    // For sake of simplicity, let's hope this never fails
    libvmdk_handle_initialize(&this->handle, &this->errorH);
    this->handleError(1);
  }

  // Destructor
  ~VMDK() {
    if (this->handle != NULL) {
      libvmdk_handle_close(this->handle, &this->errorH);
      libvmdk_handle_free(&this->handle, NULL);
      this->handle = NULL;
    }
  }

  // API from here on
  const char *getVersion() {
    return libvmdk_get_version();
  }

  int checkFileSignature(std::string filename) {
    return this->handleError(
      libvmdk_check_file_signature(filename.c_str(), &this->errorH)
    );
  }

  int open(std::string filename) {
    return this->handleError(
      libvmdk_handle_open(this->handle, filename.c_str(), LIBVMDK_OPEN_READ, &this->errorH)
    );
  }

  int openExtentDataFiles() {
    return this->handleError(
      libvmdk_handle_open_extent_data_files(this->handle, &this->errorH)
    );
  }

  int getNumberOfExtents() {
    int numberOfExtents = -1, ret;
    ret = libvmdk_handle_get_number_of_extents(this->handle, &numberOfExtents, &this->errorH);
    return this->handleError(
      ret == -1 ? ret : numberOfExtents
    );
  }

  VMDKExtent *getExtent(int index) {
    int ret;
    libvmdk_extent_descriptor_t *extent = NULL;
    ret = libvmdk_handle_get_extent_descriptor(this->handle, index, &extent, &this->errorH);
    if (ret == -1) {
      this->handleError(ret);
      return new VMDKExtent(this->handle, extent, strdup(this->errorString));
    }
    return new VMDKExtent(this->handle, extent, NULL);
  }

  long seek(long offset, int whence = SEEK_SET) {
    return this->handleError(
      (long)libvmdk_handle_seek_offset(this->handle, (off64_t)offset, whence, &this->errorH)
    );
  }

  long getOffset() {
    off64_t offset = 0;
    int ret;
    ret = libvmdk_handle_get_offset(this->handle, &offset, &this->errorH);
    return this->handleError(
      ret == -1 ? (long)ret : (long)offset
    );
  }

  long read(go_bytebuffer *buffer) {
    ssize_t ret = 0;
    ret = libvmdk_handle_read_buffer(this->handle, buffer->buffer, buffer->size, &this->errorH);
    return this->handleError((long)ret);
  }

  long readOffset(go_bytebuffer *buffer, long offset) {
    ssize_t ret;
    ret = libvmdk_handle_read_buffer_at_offset(this->handle, buffer->buffer, buffer->size, (off64_t)offset, &this->errorH);
    return this->handleError((long)ret);
  }

  int getDiskType() {
    int diskType = -1, ret;
    ret = libvmdk_handle_get_disk_type(this->handle, &diskType, &this->errorH);
    return this->handleError(
      ret == -1 ? ret : diskType
    );
  }

  long getMediaSize() {
    size64_t mediaSize = 0;
    int ret;
    ret = libvmdk_handle_get_media_size(this->handle, &mediaSize, &this->errorH);
    return this->handleError(
      ret == -1 ? (long)ret : (long)mediaSize
    );
  }

};

// --------
}
