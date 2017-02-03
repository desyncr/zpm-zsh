# vim: set ts=2 sw=2 expandtab:

Command without arguments gives an error if there are no plugins.

  $ zpm reset
  $ zpm
  Usage:
  \tzpm 'zsh-users/zsh-syntax-highlighting' (esc)
  
  Available commands:
  \tzpm reset (esc)
  \tzpm list (esc)
  \tzpm update (esc)
  \tzpm help (esc)
  [1]

Command `list` without any plugin registered show a message.

  $ zpm reset
  $ zpm list
  Nothing to show.
  [255]

Can create a new list.

  $ zpm reset
  $ zpm "zsh-users/zsh-syntax-highlighting"
  Installing zsh-users/zsh-syntax-highlighting... Done.

  $ cat $HOME/.zpm-init.zsh
  source .*/.zpm/plugins/zsh-users/zsh-syntax-highlighting/zsh-syntax-highlighting.*.zsh (re)
  fpath\+=.*/.zpm/plugins/zsh-users/zsh-syntax-highlighting/ (re)
  PATH=.*/.zpm/plugins/zsh-users/zsh-syntax-highlighting:\$PATH (re)
  autoload -Uz compinit; compinit -iCd \$HOME/.zcompdump (re)

Subsequentially add a new item to the list.

  $ zpm "rupa/z"
  Installing rupa/z... Done.
  $ cat $HOME/.zpm-init.zsh
  source .*/.zpm/plugins/zsh-users/zsh-syntax-highlighting/zsh-syntax-highlighting.*.zsh (re)
  fpath\+=.*/.zpm/plugins/zsh-users/zsh-syntax-highlighting/ (re)
  PATH=.*/.zpm/plugins/zsh-users/zsh-syntax-highlighting:\$PATH (re)
  source .*/.zpm/plugins/rupa/z/z.sh (re)
  fpath\+=.*/.zpm/plugins/rupa/z/ (re)
  PATH=.*/.zpm/plugins/rupa/z:\$PATH (re)
  autoload -Uz compinit; compinit -iCd \$HOME/.zcompdump (re)

List command show list of installed plugins.

  $ zpm list
  zsh-users/zsh-syntax-highlighting .* (re)
  rupa/z .* (re)

Already cloned plugins show still display "installing..." text.

  $ rm -rf ~/.zpm/plugins/rupa/
  $ zpm reset
  $ zpm "rupa/z"
  Installing rupa/z... Done.

  $ zpm "rupa/z"
  Installing rupa/z... Done.

Do not allow duplicate plugin entries/items.

#  $ zpm reset
#  $ zpm "rupa/z" > /dev/null
#  $ zpm list
#  rupa/z

  $ zpm "rupa/z" > /dev/null
  $ zpm list
  rupa/z .* (re)

Can install multiple plugins under the same user.

  $ zpm reset
  $ zpm "zsh-users/zsh-syntax-highlighting"
  Installing zsh-users/zsh-syntax-highlighting... Done.
  $ zpm "zsh-users/zsh-autosuggestions"
  Installing zsh-users/zsh-autosuggestions... Done.
  $ ls $HOME/.zpm/plugins/zsh-users/
  zsh-autosuggestions (re)
  zsh-syntax-highlighting (re)

Remove spurius files.

  $ zpm reset

