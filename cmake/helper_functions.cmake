# ── High-level: ensure exactly one option in a group is ON ────────────────────
# Usage:
#   resolve_mutex_option(OPT_DEFAULT OPT_B OPT_C ...)          ← LOCAL (default)
#   resolve_mutex_option(CACHE OPT_DEFAULT OPT_B OPT_C ...)    ← CACHE + FORCE
#
#   - The first option in the list is both the default AND has the highest
#     priority (if multiple options happen to be ON, the first one wins).
#   - All other options are turned OFF.
#   - If none is ON, the first option is turned ON (the default).
#
# Two modes:
#   LOCAL  (default)  – set(… PARENT_SCOPE), regular variable, no cache pollution.
#   CACHE             – set(… CACHE BOOL "" FORCE), survives re-runs.
#
# Call this once per group after declaring the options — it eliminates the
# repetitive if/elseif/set(… CACHE … FORCE) chains.
function(resolve_mutex_option)
    # ── Determine mode from the first argument ───────────────────────────────
    # _use_cache is a plain boolean flag (TRUE/FALSE), not a CMake keyword,
    # to avoid "set given invalid arguments for CACHE mode" errors.
    set(_use_cache FALSE)
    if (ARGV0 STREQUAL "CACHE")
        set(_use_cache TRUE)
    endif ()

    set(_found OFF)
    set(_is_first_arg TRUE)

    foreach (_opt IN LISTS ARGN)
        # Skip the "CACHE" keyword itself so it isn't treated as an option name
        if (_is_first_arg AND _use_cache)
            set(_is_first_arg FALSE)
            continue()
        endif ()
        set(_is_first_arg FALSE)

        # Branch on mode to avoid duplicating the loop body
        if (${_opt} AND NOT _found)
            set(_found ON)
            if (_use_cache)
                set(${_opt} ON CACHE BOOL "" FORCE)
            else ()
                set(${_opt} ON PARENT_SCOPE)
            endif ()
        else ()
            if (_use_cache)
                set(${_opt} OFF CACHE BOOL "" FORCE)
            else ()
                set(${_opt} OFF PARENT_SCOPE)
            endif ()
        endif ()
    endforeach ()

    # ── No option was ON → enable the default (first *real* argument) ────────
    if (NOT _found)
        if (_use_cache)
            list(GET ARGN 1 _first)
        else ()
            list(GET ARGN 0 _first)
        endif ()

        if (_use_cache)
            set(${_first} ON CACHE BOOL "" FORCE)
        else ()
            set(${_first} ON PARENT_SCOPE)
        endif ()
    endif ()
endfunction()
