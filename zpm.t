# vim: set ts=4 sw=4 expandtab:

Command without arguments gives an error if there are no plugins.
    
    $ ./zpm reset
    $ ./zpm
    zpm 'zsh-users/zsh-syntax-highlighting'
    [1]    

Command `list` without any plugin registered show a message.

    $ ./zpm reset
    $ ./zpm list
    Nothing to show.

Can create a new list.

    $ ./zpm reset
    $ ./zpm "zsh-users/zsh-syntax-highlighting"
    $ cat $HOME/.zpmrc
    source .*/.zpm/plugins/zsh-users/zsh-syntax-highlighting/zsh-syntax-highlighting.zsh (re)

Subsequentially add a new item to the list.

    $ ./zpm "rupa/z"
    $ cat $HOME/.zpmrc
    source .*/.zpm/plugins/zsh-users/zsh-syntax-highlighting/zsh-syntax-highlighting.zsh (re)
    source .*/.zpm/plugins/rupa/z/z.sh (re)


List command show list of installed plugins.

    $ ./zpm list
    zsh-users/zsh-syntax-highlighting
    rupa/z

Already cloned plugins show still display "installing..." text.

    $ rm -rf ~/.zpm/plugins/rupa/
    $ ./zpm reset
    $ ./zpm "rupa/z"
    Installing rupa/z... Done.
    
    $ ./zpm "rupa/z"
    Installing rupa/z... Done.

Do not allow duplicate plugin entries/items.

#    $ ./zpm reset
#    $ ./zpm "rupa/z" &> /dev/null
#    $ ./zpm list
#    rupa/z

    $ ./zpm "rupa/z" &> /dev/null
    $ ./zpm list
    rupa/z

Can install multiple plugins under the same user.

    $ ./zpm reset
    $ ./zpm "zsh-users/zsh-syntax-highlighting"
    $ ./zpm "zsh-users/zsh-autosuggestions"
    $ ls -l $HOME/.zpm/plugins/zsh-users/
    zsh-syntax-highlighting
    zsh-autosuggestions

Remove spurius files.

    $ ./zpm reset

