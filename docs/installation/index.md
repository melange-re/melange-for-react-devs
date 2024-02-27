# Installation

## Prerequisites

- You are running a Unix system
  - Windows users are encouraged to install [Windows Subsystem for
Linux](https://learn.microsoft.com/en-us/windows/wsl/)
- You have a recent version of, and know how to use:
  - [Node.js](https://nodejs.org/)
  - [git](https://git-scm.com/)
  - A code editor such as [Visual Studio Code](https://code.visualstudio.com/)
    - There are also [many other editors](https://melange.re/v3.0.0/getting-started.html#editor-integration)
      which can support OCaml

## Opam

We need [opam](https://opam.ocaml.org/), the OCaml Package Manager. There are
many ways to install it depending on your platform, but let's go with the
simplest method:

```bash
bash -c "sh <(curl -fsSL https://raw.githubusercontent.com/ocaml/opam/master/shell/install.sh)"
opam init
eval $(opam env)
```

While `opam init` is running, it will prompt you with something like

```
Do you want opam to modify ~/.profile?
```

Type `y` to agree.

After the installation completes, run

```
opam --version
```

to verify that it succeeded.

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
