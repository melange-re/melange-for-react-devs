# Installation

## Prerequisites

- You are running a Unix system
  - Windows users are encouraged to install [Windows Subsystem for
Linux](https://learn.microsoft.com/en-us/windows/wsl/)
- You have a recent version of, and know how to use:
  - [Node.js](https://nodejs.org/)
  - [git](https://git-scm.com/)
  - A code editor such as [Visual Studio Code](https://code.visualstudio.com/)
    - There are also [many other editors](https://melange.re/v4.0.0/getting-started.html#editor-integration)
      which can support OCaml

## GNU `patch`

Your system might already have a `patch` binary, but we need `patch` to
specifically run [GNU patch](https://savannah.gnu.org/projects/patch/). Run this
in your shell:

```bash
patch --version
```

If it returns something like

```text
GNU patch 2.7.6
Copyright (C) 2003, 2009-2012 Free Software Foundation, Inc.
Copyright (C) 1988 Larry Wall

License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.

Written by Larry Wall and Paul Eggert
```

then you're good to go. Otherwise, you need to install GNU patch and make sure
that running `patch` will invoke it. How you do it depends on your operating
system. For example, on Macs you can install from [Homebrew](https://brew.sh/)
by running `brew install gpatch`.

## Opam

We need [opam](https://opam.ocaml.org/), the OCaml Package Manager. There are
many ways to [install](https://opam.ocaml.org/doc/Install.html) it depending on
your platform, but let's go with the simplest method:

```shell
bash -c "sh <(curl -fsSL https://raw.githubusercontent.com/ocaml/opam/master/shell/install.sh)"
opam init
```

While `opam init` is running, it will prompt you with something like

```
Do you want opam to modify ~/.zshrc? [N/y/f]
```

Type `y` to agree.

::: warning

If it asks

```
Do you want opam to modify ~/.profile? [N/y/f]
```

You should enter `f` and then enter `~/.bashrc` as the file to be modified.
There's a [bug when installing the shell hook when running Bash or Bourne
shell](https://github.com/ocaml/opam/issues/5819).

:::

### Verify that `opam` was correctly installed

After the installation completes, run

```shell
opam switch
```

If you see this message at the bottom, then the shell hook wasn't installed
correctly:

```
[WARNING] The environment is not in sync with the current switch.
          You should run: eval $(opam env)
```

To get rid of the warning, you should run setup again:

```shell
opam init --reinit
```

Then follow the instructions for the warning section above.

## Download and run the starter project

Let's make sure that everything works by downloading and running our project
template
[melange-for-react-devs-template](https://github.com/melange-re/melange-for-react-devs-template).

```bash
git clone https://github.com/melange-re/melange-for-react-devs-template
cd melange-for-react-devs-template
npm run init
npm run build
npm run serve
```

While `npm run init` is running, consider grabbing some coffee or other
beverage, as it might take a while to fetch all the dependencies and build them.
The last command, `npm run serve`, should open a tab in your default browser which
points to <a href="http://localhost:5174/" target="_blank" rel="noreferrer
noopener">http://localhost:5174/</a> and shows you a typical "Hello World" page.
If you see this page, then the project was successfully installed!

## Visual Studio Code Extension

1. Open the Extensions tab in Visual Studio Code and search for "ocaml".
1. Install the [OCaml
Platform](https://marketplace.visualstudio.com/items?itemName=ocamllabs.ocaml-platform)
extension from OCaml Labs.
1. To verify that the extension worked, open the melange-for-react-devs project
in Visual Studio Code. OCaml code should be syntax highlighted, and you should
see type annotations when you hover over variables.
1. To enable auto-formatting, open your [User Settings
JSON file](https://code.visualstudio.com/docs/getstarted/settings#_settingsjson)
and add this snippet:

```json
"[reason]": {
  "editor.formatOnSave": true,
  "editor.defaultFormatter": "ocamllabs.ocaml-platform"
}
```
