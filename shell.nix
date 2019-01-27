with import <nixpkgs> { };
let
  dirprod = callPackage ./. { };

in mkShell rec {
  name = dirprod.name;
  buildInputs = dirprod.nativeBuildInputs ++ dirprod.buildInputs ++ [ dpkg ];
  shellHook = ''
    echo Welcome to ${name} environment
  '';
}
