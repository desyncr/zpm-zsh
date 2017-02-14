# vim: set ts=2 sw=2 expandtab:

Can't remove non-installed plugin
  $ zpm reset
  $ zpm remove non_installed_plugin
  Plugin "non_installed_plugin" is not installed.
  [1]

Plugin parent directory is not removed if not empty
  $ zpm reset
  $ zpm "zsh-users/zsh-syntax-highlighting" > /dev/null
  $ zpm "zsh-users/zsh-autosuggestions"     > /dev/null
  $ zpm remove "zsh-users/zsh-syntax-highlighting" > /dev/null
  $ ls $HOME/.zpm/plugins/zsh-users/
  zsh-autosuggestions

Plugin directory is properly unlinked after remove and parent directory is properly removed

  $ zpm list
  zsh-users/zsh-autosuggestions@.* (re)

  $ zpm "zsh-users/zsh-syntax-highlighting" > /dev/null
  $ zpm remove "zsh-users/zsh-syntax-highlighting" > /dev/null
  $ zpm remove "zsh-users/zsh-autosuggestions" > /dev/null

  $ [ -d $HOME/.zpm/plugins/zsh-users/zsh-syntax-highlighting ]
  [1]

  $ [ -d $HOME/.zpm/plugins/zsh-users/ ]
  [1]

After being removed, plugin is not listed anymore
  $ zpm list
  Nothing to show.
  [1]

