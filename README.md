#->lsh
##A learning shell
This is a simple, stripped-down learning shell that impliments history and a few shell builtins
It aims to be well-documented, and with time the shell will impliment job control and buildin features
in a fashon more in line with modern shells such as zsh.

Until then, hopefully it may help others who are doing this for a class assignment, such as myself.

##Building:

```shell
./autogen.sh
./configure
make
```

##Running, exiting

```shell
./shell/lsh

->exit
```



##TODO:
* Implement POSIX-compliant job control, and shell initiialization, using process groups and tcsetpgrp()
* ~~Don't put history commands in the history file~~
* Commands run from history will ignore &. Fix this
