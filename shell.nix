with import <nixpkgs> { };
let
  dirprod = callPackage ./. { };

in mkShell rec {
  name = "direct_product";
  buildInputs = dirprod.nativeBuildInputs ++ dirprod.buildInputs ++ [ dpkg ];
  shellHook = ''
    echo Welcome to ${name} environment
  '';
}
