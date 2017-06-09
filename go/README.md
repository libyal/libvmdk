# Go language bindings for libvmdk

## Easy start

You will need to have built libyaml before and have at least Go 1.6 and Swig 3.0.

```
mkdir -p $GOPATH/src/github.com/libyal
cd $GOPATH/src/github.com/libyal
git clone https://github.com/libyal/libvmdk.git
cd libyaml/go
go get -t
go test
```

See libvmdk_test.go for examples on how to use the bindings.
