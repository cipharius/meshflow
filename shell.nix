{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  nativeBuildInputs = with pkgs; [
    gcc
    pkgconfig
    gnumake
    xlibs.libX11
    glfw3
    mesa.dev
    xorg.libxcb.dev
    xorg.libXau.dev
    xorg.libXdmcp.dev

    gdb
    tig
  ];
}
