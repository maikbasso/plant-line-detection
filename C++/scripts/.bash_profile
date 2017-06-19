# ~/.bashrc: executed by bash(1) for non-login shells.
# see /usr/share/doc/bash/examples/startup-files (in the package bash-doc)
# for examples

#@Author: Maik Basso <maik@maikbasso.com.br>
#Alias for PLD project
alias open-pld="cd ~/Dropbox/Faculdade/Pós-Graduação/Mestrado/PPGEE\ -\ UFRGS/Artigos/PlantLineDetection/plant-line-detection/"
alias pld-compile="open-pld && cd C++/build && cmake .. && make && ./main && cd ../.."
alias pld-run="open-pld && cd C++/build && ./main && cd ../.."

# $ source .bashrc
# ...