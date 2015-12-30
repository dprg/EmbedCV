
# Release 0.3 of EmbedCV
AC_INIT(EmbedCV, 0.3, cjang@ix.netcom.com)

# Build as a C library
AC_PROG_CC

# Check if compiler is gcc
if test "$GCC" = "yes"
then
	# highest optimization for GCC, settings different for other compilers
	CFLAGS="-O3 -funroll-loops"
fi

# Check for tools besides the compiler
AC_CHECK_PROG(AR, ar, ar, missing)
AC_CHECK_PROG(RANLIB, ranlib, ranlib, missing)
AC_CHECK_PROG(STRIP, strip, strip, missing)
AC_CHECK_PROG(INSTALL, install, install, missing)




# Check for IJG libjpeg library (hopefully version 6b)
private_check_libjpeg=yes
AC_CHECK_HEADER(jpeglib.h, [], [private_check_libjpeg=no])
AC_CHECK_LIB(jpeg, jpeg_start_decompress, [], [private_check_libjpeg=no])




# Check for enable shared library build option
private_enable_shared=no
AC_ARG_ENABLE(shared,
	[  --enable-shared         build shared library (default is static archive only)],
	[
		if test "x$enableval" = "xyes"
		then
			private_enable_shared=yes
		fi
	]
)

# Check for enable sample code build option
private_enable_samples=no
AC_ARG_ENABLE(samples,
	[  --enable-samples        build sample code (default is no sample code)],
	[
		if test "x$enableval" = "xyes"
		then
			private_enable_samples=yes
		fi
	]
)

# Check for disable explicit loop unrolling option
private_enable_explicit_loop_unroll=yes
AC_ARG_ENABLE(loop,
	[  --disable-loop          disable explicit loop unrolling (reduce code size)],
	[
		if test "x$enableval" = "xno"
		then
			private_enable_explicit_loop_unroll=no
		fi
	]
)

# Check for disable PPM option
private_enable_ppm=yes
AC_ARG_ENABLE(ppm,
	[  --disable-ppm           disable PPM support (then make library smaller)],
	[
		if test "x$enableval" = "xno"
		then
			private_enable_ppm=no
		fi
	]
)

# Check for disable JPEG option
private_with_jpeg=yes
AC_ARG_WITH(jpeg,
	[  --without-jpeg          disable JPEG support (then IJG libjpeg unnecessary)],
	[
		if test "x$withval" = "xno"
		then
			private_with_jpeg=no
		fi
	]
)




# Disable the libjpeg library if it is not needed
if test "$private_with_jpeg" = "no"
then
	AC_MSG_WARN([JPEG codec support is disabled])

	AC_SUBST(WITH_JPEG, WITHOUT_JPEG)
	AC_SUBST(CODEC_JPEG_FILES, "")
	LDFLAGS="${LDFLAGS}"

# JPEG is enabled
else
	# Need JPEG but no installed library found
	if test "$private_check_libjpeg" = "no"
	then
		AC_MSG_ERROR([*** IJG JPEG library not found, please install libjpeg version 6b ***])
	fi

	AC_SUBST(WITH_JPEG, WITH_JPEG)
	AC_SUBST(CODEC_JPEG_FILES, "codecjpeg.o wrap_transupp.o")
	LDFLAGS="${LDFLAGS} -ljpeg"
fi


# Disable PPM support if it is not needed
if test "$private_enable_ppm" = "no"
then
	AC_MSG_WARN([PPM codec support is disabled])

	AC_SUBST(WITH_PPM, WITHOUT_PPM)
	AC_SUBST(CODEC_PPM_FILES, "")

# PPM is enabled
else
	AC_SUBST(WITH_PPM, WITH_PPM)
	AC_SUBST(CODEC_PPM_FILES, "codecppm.o")
fi


# Should sample code be built?
if test "$private_enable_samples" = "yes"
then
	# Only build sample code if both JPEG and PPM support are present
	if test "$private_with_jpeg" = "yes" && test "$private_enable_ppm" = "yes"
	then
		# build the sample code
		AC_SUBST(SAMPLE_BUILD_NOTHING_TARGET, "")
	else
		AC_MSG_WARN([Sample code will not be built])
		AC_MSG_WARN([Both JPEG and PPM support are required for sample code])
		AC_SUBST(SAMPLE_BUILD_NOTHING_TARGET, "buildnothing:")
	fi
else
	AC_MSG_WARN([Sample code will not be built])
	AC_SUBST(SAMPLE_BUILD_NOTHING_TARGET, "buildnothing:")
fi


# Explicit loop unrolling can be disabled (not recommended)
if test "$private_enable_explicit_loop_unroll" = "no"
then
	AC_SUBST(EXPLICIT_UNROLL_LOOP, 0)

# Allow explicit loop unrolling
else
	AC_SUBST(EXPLICIT_UNROLL_LOOP, 1)
fi


# Building the shared library is optional
if test "$private_enable_shared" = "no"
then
	# do not build the shared library
	AC_SUBST(BUILD_SHARED_TARGET, "")
	AC_SUBST(INSTALL_SHARED_TARGET, "")
else
	# build the shared library
	AC_SUBST(BUILD_SHARED_TARGET, "libembedcv.so")
	AC_SUBST(INSTALL_SHARED_TARGET, "install-shared")
fi


AC_OUTPUT(include/embedcv.h)
AC_OUTPUT(src/Makefile)
AC_OUTPUT(sample/Makefile)
AC_OUTPUT(Makefile)


# Summary
AC_MSG_NOTICE([--> Installing to:            ${prefix}])
AC_MSG_NOTICE([--> Compiler:                 ${CC}])
AC_MSG_NOTICE([--> Compile flags:            ${CFLAGS}])


# Building the shared library is optional
if test "$private_enable_shared" = "no"
then
AC_MSG_NOTICE([--> Build shared library:     NO])
else
AC_MSG_NOTICE([--> Build shared library:     YES])
fi

# Is sample code built?
if test "$private_enable_samples" = "yes" && test "$private_with_jpeg" = "yes" && test "$private_enable_ppm" = "yes"
then
AC_MSG_NOTICE([--> Build sample code:        YES])
else
AC_MSG_NOTICE([--> Build sample code:        NO])
fi

# Is loop unrolling explicit and forced with macros?
if test "$private_enable_explicit_loop_unroll" = "yes"
then
AC_MSG_NOTICE([--> explicit loop unrolling:  YES])
else
AC_MSG_NOTICE([--> explicit loop unrolling:  NO])
fi

# Is JPEG support built into the library
if test "$private_with_jpeg" = "yes"
then
AC_MSG_NOTICE([--> JPEG support included:    YES])
else
AC_MSG_NOTICE([--> JPEG support included:    NO])
fi

# Is PPM support built into the library
if test "$private_enable_ppm" = "yes"
then
AC_MSG_NOTICE([--> PPM support included:     YES])
else
AC_MSG_NOTICE([--> PPM support included:     NO])
fi

