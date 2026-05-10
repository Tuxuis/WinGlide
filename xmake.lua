add_rules("mode.debug", "mode.release")

target("WinGlide")
    set_kind("binary")
    add_files("src/*.cpp")

    add_syslinks("user32", "gdi32")
    add_languages("c++17")

    -- Force console to hide on startup
    add_ldflags("-mwindows", { force = true })