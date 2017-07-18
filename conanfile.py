from conans import ConanFile, CMake, tools
import os


class GPCBin2CConan(ConanFile):
    name = "gpcbin2c"
    version = "0.1"
    license = "MIT"
    url = "<Package recipe repository url here, for issues about the package>"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=False"
    generators = "cmake"
    exports_sources = "bin2c.cpp", "CMakeLists.txt"

    def build(self):
        cmake = CMake(self)
        shared = "-DBUILD_SHARED_LIBS=ON" if self.options.shared else ""
        self.run('cmake . %s %s' % (cmake.command_line, shared))
        self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        #self.copy("*.h", dst="include", src="hello")
        #self.copy("*hello.lib", dst="lib", keep_path=False)
        #self.copy("*.dll", dst="bin", keep_path=False)
        #self.copy("*.so", dst="lib", keep_path=False)
        #self.copy("*.dylib", dst="lib", keep_path=False)
        #self.copy("*.a", dst="lib", keep_path=False)
        self.copy("gpcbin2c.exe", dst="bin", keep_path=False)
        self.copy("gpcbin2c", dst="bin", keep_path=False)

    def package_info(self):
        #self.cpp_info.libs = ["hello"]
        pass