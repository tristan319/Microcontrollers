idf_component_register(
    SRCS "main.c" "wifi.c" "webserver.c"
    INCLUDE_DIRS "."
    EMBED_FILES "index.html"
)