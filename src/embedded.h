
/* Declarations for global variables containing embedded content,
 * such as shader source code.
 * See the compilation script "_comp.py" or the EMBEDDED macro for more. */

#ifndef WHYCRYSTALS_HEADER_EMBEDDED__
#define WHYCRYSTALS_HEADER_EMBEDDED__

/* If this macro is used as would be the extern keyword in a variable
 * declaration, and if filename_ is a string literal, then the compilation
 * script "_comp.py" will parse the variable declaration as a special
 * declaration and then generates code that sets the variable to the content
 * of the file found at filename_ in the generated source file "embedded.c".
 * The variable declaration is expected to be in the global scope and to have
 * a type compatible with the generated literal.
 * The escape_mode_ parameter can be TEXT, BINARY or SIZE,
 * it influences the way the content to embed is escaped into a literal. */
#define EMBEDDED(filename_, escape_mode_) extern
#define TEXT /* Escapes the file content as a string literal. */
#define BINARY /* Escapes the file content as an array of bytes. */
#define SIZE /* Just produces an integer literal of the size in bytes. */

EMBEDDED("shaders/visuals.vert", TEXT) const char g_shader_visuals_vert[];
EMBEDDED("shaders/visuals.geom", TEXT) const char g_shader_visuals_geom[];
EMBEDDED("shaders/visuals.frag", TEXT) const char g_shader_visuals_frag[];

#endif /* WHYCRYSTALS_HEADER_EMBEDDED__ */
