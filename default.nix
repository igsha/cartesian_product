{ stdenv, writeText, cmake, boost, gcc, catch2 }:

let
  cmakeVersionRegex = ".*project\\(.*VERSION[[:space:]]+([[:digit:]\\.]+).*";
  version = builtins.head (builtins.match cmakeVersionRegex (builtins.readFile ./CMakeLists.txt));

in stdenv.mkDerivation rec {
  name = "${pname}-${version}";
  pname = "dirprod";
  inherit version;

  src = ./.;

  setupHook = writeText "setupHook.sh" ''
    set${pname}Dir() {
      if [[ -r "$1/share/cmake/${pname}/${pname}-config.cmake" ]]; then
        export ${pname}_DIR="$1"
      fi
    }

    addEnvHooks "$targetOffset" set${pname}Dir
  '';

  nativeBuildInputs = [ cmake setupHook boost gcc ];
  buildInputs = [ catch2 ];
  doCheck = true;

  meta = with stdenv.lib; {
    description = "A header-only C++ library to provide direct product of containers and ranges";
    homepage = https://github.com/igsha/dirprod;
    license = licenses.mit;
    platforms = platforms.all;
    maintainers = with maintainers; [ igsha ];
  };
}
