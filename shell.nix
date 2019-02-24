with import <nixpkgs> { };
let
  dirprod = callPackage ./. { };

in mkShell rec {
  name = dirprod.name;
  buildInputs = dirprod.nativeBuildInputs ++ dirprod.buildInputs ++ [ dpkg valgrind gdb ];
  hardeningDisable = [ "all" ];
  shellHook = ''
    echo Welcome to ${name} environment
  '';
}
