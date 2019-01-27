{ pkgs ? import <nixpkgs> {} }:

let
  cmakeVersionRegex = ".*project\\(.*VERSION[[:space:]]+([[:digit:]\.]+).*";
  version = builtins.head (builtins.match cmakeVersionRegex (builtins.readFile ./CMakeLists.txt));

in pkgs.stdenv.mkDerivation rec {
  name = "${pname}-${version}";
  pname = "dirprod";
  inherit version;

  src = ./.;

  setupHook = pkgs.writeText "setupHook.sh" ''
    set${pname}Dir() {
      if [[ -r "$1/share/cmake/${pname}/${pname}-config.cmake" ]]; then
        export ${pname}_DIR="$1"
      fi
    }

    addEnvHooks "$targetOffset" set${pname}Dir
  '';

  nativeBuildInputs = with pkgs; [ cmake setupHook boost gcc ];
  buildInputs = with pkgs; [ catch2 ];
  doCheck = true;

  meta = with pkgs.stdenv.lib; {
    description = "A header-only C++ library to provide direct product of containers and ranges";
    homepage = https://github.com/igsha/direc_product;
    license = licenses.mit;
    platforms = platforms.all;
    maintainers = with maintainers; [ igsha ];
  };
}
