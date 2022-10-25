cd "${SPRING_DIR}/conan"

CONAN_CMAKE_TOOLCHAIN_FILE="/scripts/${PLATFORM}.cmake"
if [ "${MYBUILDTYPE}" == "DEBUG" ]; then
    PROFILE_BUILDTYPE="DEBUG"
else
    PROFILE_BUILDTYPE="RELEASE"
fi
conan install "./conanfile-${PLATFORM}.txt" --profile "./conanprofile-${PLATFORM}-${PROFILE_BUILDTYPE}"
