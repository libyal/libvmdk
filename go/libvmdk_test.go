package libvmdk

import (
	"github.com/stretchr/testify/assert"
	"os"
	"testing"
)

func TestGetVersion(t *testing.T) {
	var vmdk = NewVMDK()
	defer DeleteVMDK(vmdk)

	ret := vmdk.GetVersion()
	assert.NotEqual(t, ret, "", "empty version")
}

func TestGetMediaSize(t *testing.T) {
	var vmdk = NewVMDK()
	defer DeleteVMDK(vmdk)

	ret := vmdk.Open("LibVMDKTest.vmdk")
	assert.NotEqual(t, ret, -1, "failed to open test file")

	size := vmdk.GetMediaSize()
	assert.NotEqual(t, size, -1, "failed to get media size")
	assert.NotEqual(t, size, 0, "media size is zero")
}

func TestExtents(t *testing.T) {
	var vmdk = NewVMDK()
	defer DeleteVMDK(vmdk)

	ret := vmdk.Open("LibVMDKTest.vmdk")
	assert.NotEqual(t, ret, -1, vmdk.Error())

	ret = vmdk.GetNumberOfExtents()
	assert.NotEqual(t, ret, -1, vmdk.Error())
	assert.Equal(t, ret, 1, "number of extents doesn't match 1")

	extent := vmdk.GetExtent(1)
	assert.NotEmpty(t, extent.Error(), vmdk.Error())

	extent = vmdk.GetExtent(0) // non-existant extent, should error
	assert.Empty(t, extent.Error(), vmdk.Error())

	extentFilename := extent.GetFilename()
	assert.Equal(t, extentFilename, "LibVMDKTest.vmdk", "extent filename is wrong")

}

func TestReadFromBeginning(t *testing.T) {
	var vmdk = NewVMDK()
	defer DeleteVMDK(vmdk)

	ret := vmdk.Open("LibVMDKTest.vmdk")
	assert.NotEqual(t, ret, -1, "failed to open test file")

	ret = vmdk.OpenExtentDataFiles()
	assert.NotEqual(t, ret, -1, "failed to open extent data files")

	// 8 byte buffer
	buffer := make([]byte, 8, 8)
	bytesRead := vmdk.Read(buffer)
	if bytesRead == -1 {
		assert.Fail(t, vmdk.Error())
	}
	assert.Equal(t, buffer, []byte{0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0x00, 0x00, 0x00}, "buffer contents wrong")
}

func TestReadFromOffset(t *testing.T) {
	var vmdk = NewVMDK()
	defer DeleteVMDK(vmdk)

	ret := vmdk.Open("LibVMDKTest.vmdk")
	assert.NotEqual(t, ret, -1, "failed to open test file")

	ret = vmdk.OpenExtentDataFiles()
	assert.NotEqual(t, ret, -1, "failed to open extent data files")

	// 8 byte buffer
	buffer := make([]byte, 8, 8)
	bytesRead := vmdk.ReadOffset(buffer, 512)
	if bytesRead == -1 {
		assert.Fail(t, vmdk.Error())
	}
	assert.Equal(t, buffer, []byte{0xDE, 0xAD, 0xCA, 0xFE, 0x00, 0x00, 0x00, 0x00}, "buffer contents wrong")
}

func TestSeekAndRead(t *testing.T) {
	var vmdk = NewVMDK()
	defer DeleteVMDK(vmdk)

	ret := vmdk.Open("LibVMDKTest.vmdk")
	assert.NotEqual(t, ret, -1, "failed to open test file")

	ret = vmdk.OpenExtentDataFiles()
	assert.NotEqual(t, ret, -1, "failed to open extent data files")

	// 8 byte buffer
	buffer := make([]byte, 8, 8)
	vmdk.Seek(int64(1024), os.SEEK_SET)
	bytesRead := vmdk.Read(buffer)
	if bytesRead == -1 {
		assert.Fail(t, vmdk.Error())
	}
	assert.Equal(t, buffer, []byte{0xDE, 0xAD, 0xC0, 0xDE, 0x00, 0x00, 0x00, 0x00}, "buffer contents wrong")
}

func TestFileSignature(t *testing.T) {
	var vmdk = NewVMDK()
	defer DeleteVMDK(vmdk)

	ret := vmdk.CheckFileSignature("LibVMDKTest.vmdk")
	assert.Equal(t, ret, 1, "invalid signature")
}
