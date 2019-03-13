dirprod
=======

Implements cartesian product (direct product) to reduce for-loops in programs

.. code:: cpp

   #include <dirprod/range.hpp>

   enum Modes { DC, PLANAR, HORIZONTAL, VERTICAL, ANGLE45, MODES_SIZE };
   enum Components { Y, Cb, Cr, COMPONENTS_SIZE };

   // simple approach
   for (int boolean = 0; boolean < 2; ++boolean)
   {
       for (int mode = DC; mode < MODES_SIZE; ++mode)
       {
           for (int comp = Y; comp < COMPONENTS_SIZE; ++comp)
           {
               auto choice = std::make_tuple(Components(comp), bool(boolean), Modes(mode));
               // process choice
           }
       }
   }

   // using cartesian product
   auto modes = boost::counting_range(int(DC), int(MODES_SIZE)) | boost::adaptors::transformed(+[](int x)
   {
       return static_cast<Modes>(x);
   });
   std::list<bool> boolean{{false, true}};

   auto enumeration = dirprod::range(std::vector{Y, Cb, Cr}, boolean, modes);
   // iterate through all possible combinations
   for (const auto& [comp, b, mode] : enumeration)
   {
       // process choice
   }

Installation
============

Use latest ``<version>`` from https://github.com/igsha/dirprod/releases.

Ubuntu way (install and upgrade)::

   wget -q https://github.com/igsha/dirprod/releases/download/v<vection>/dirprod-<version>-Linux.deb
   sudo apt install ./dirprod-<version>-Linux.deb

NixOS way (from terminal)::

   nix-shell -E 'with import <nixpkgs> {}; mkShell { buildInputs = [ \
      cmake gcc boost \
      (callPackage (fetchTarball https://github.com/igsha/dirprod/archive/v<version>.tar.gz) {}) \
      ]; }'

Minimal ``shell.nix`` example::

   with import <nixpkgs> {};

   let
     dirprod-tarball = builtins.fetchTarball https://github.com/igsha/dirprod/archive/v<version>.tar.gz;
     dirprod = callPackage dirprod-tarball { };

   in mkShell {
     name = "shell";
     buildInputs = [ dirprod cmake gcc boost ];
     hardeningDisable = [ "all" ];
     shellHook = ''
       echo Welcome to minimal dirprod environment
     '';
   }

How to use it
=============

In your ``CMakeLists.txt``::

   find_package(dirprod REQUIRED)

   add_executable(myexample main.cpp)
   target_link_libraries(myexample dirprod)
