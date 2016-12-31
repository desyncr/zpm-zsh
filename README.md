# zpm - Zsh Plugin Manager

zsh plugin manager in ansi C.

### Installation

Clone this repository and then:

```sh
make

```

You'll have a `zpm` binary in the current directory, copy it to `/usr/local/bin`.

### Usage

#### Installing plugins

You first will need to install the plugins you want:

```sh
zpm "zsh-users/zsh-syntax-highlighting"
zpm "zsh-users/zsh-autosuggestions"
zpm "rupa/z"
```

#### Configure zsh

Then configure zsh to use them:

```sh
# .zshrc
source ~/.zpmrc
```

If you add or remove new plugins you will need to:

```sh
zpm --reset
```

To see a list of installed plugins:

```sh
zpm

```

