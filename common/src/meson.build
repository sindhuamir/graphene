common_dep = declare_dependency(
    sources: files(
        'avl_tree.c',
        'crypto/adapters/mbedtls_adapter.c',
        'network/hton.c',
        'network/inet_pton.c',
        'path.c',
        'stack_protector.c',
        'stdlib/printfmt.c',
        'string/atoi.c',
        'string/ctype.c',
        'string/memcmp.c',
        'string/memcpy.c',
        'string/memset.c',
        'string/strchr.c',
        'string/strcmp.c',
        'string/strlen.c',
        'string/strspn.c',
        'string/strstr.c',
        'string/toml_utils.c',
        'string/utils.c',
    ),

    compile_args: [
        '-DMBEDTLS_CONFIG_FILE="mbedtls-config.h"',
        '-DCRYPTO_USE_MBEDTLS',
    ],

    include_directories: common_inc,

    dependencies: [
        uthash_dep,
        tomlc99_dep,
        mbedtls_files_dep,
    ],
)
