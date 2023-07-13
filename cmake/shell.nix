{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShell {
    # nativeBuildInputs is usually what you want -- tools you need to run
    nativeBuildInputs = with pkgs.buildPackages; [
      cmakeMinimal
      fmt
      include-what-you-use
      SDL2
      SDL2_image
    ];
}
