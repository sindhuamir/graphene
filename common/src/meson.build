common_dep = declare_dependency(
    'avl_tree.c',
    'path.c',
    'stack_protector.c',

    'network/hton.c',
    'network/inet_pton.c',

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

    include_directories: common_inc,

    dependencies: [
        uthash_dep,
        tomlc99_dep,
        mbedtls_dep,
    ],
)
