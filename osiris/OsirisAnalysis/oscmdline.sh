#!/bin/bash
clear
export OSIRISFILES="{OSIRISFILES}"
export OSIRISTOOLS="{OSIRISFILES}/ConfigurationTools"
export PATH="{OSIRISEXEDIR}:${PATH}"
export BASH_SILENCE_DEPRECATION_WARNING=1
cd "${OSIRISTOOLS}"
export PS1='\w\n\$ '
exec /bin/bash --norc
