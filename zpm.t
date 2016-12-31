# vim: set ts=4 sw=4 expandtab:

Command without arguments gives an error if there are no plugins.
    
    $ ./zpm reset
    $ ./zpm
    zpm 'zsh-users/zsh-syntax-highlighting'
    [1]    

Can create a new list.

    $ ./zpm reset
    $ ./zpm "zsh-users/zsh-syntax-highlighting"
    $ cat $HOME/.zpmrc
    source /home/.*/.zpm/plugins/zsh-users/zsh-syntax-highlighting/zsh-syntax-highlighting.zsh (re)

Subsequentially add a new item to the list.

    $ ./zpm "rupa/z"
    $ cat $HOME/.zpmrc
    source /home/.*/.zpm/plugins/zsh-users/zsh-syntax-highlighting/zsh-syntax-highlighting.zsh (re)
    source /home/.*/.zpm/plugins/rupa/z/z.sh (re)


List command show list of installed plugins.

    $ ./zpm list
    zsh-users/zsh-syntax-highlighting
    rupa/z

Remove spurius files.

    $ ./zpm reset

