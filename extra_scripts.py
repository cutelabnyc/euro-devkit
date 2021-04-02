# Extra scripts file if needed!
Import("env")

# General options that are passed to the C and C++ compilers
env.Append(MCFLAGS=["-mthumb", "-march=armv7e-m",
                    "-mfloat-abi=hard", "-mfpu=fpv4-sp-d16"])
