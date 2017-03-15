#添加目录下所有文件
aux_source_directory(. DIR_LIB_SRCS)

#编译库
add_library(longlib ${DIR_LIB_SRCS})
