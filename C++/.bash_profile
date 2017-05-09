# ~/.bashrc: executed by bash(1) for non-login shells.
# see /usr/share/doc/bash/examples/startup-files (in the package bash-doc)
# for examples

#@Author: Maik Basso <maik@maikbasso.com.br>
#Alias for PLD project
alias open-pld="cd ~/Dropbox/Faculdade/Pós-Graduação/Mestrado/PPGEE\ -\ UFRGS/Artigos/PlantLineDetection/plant-line-detection/"
alias pld-photo-compile="open-pld && g++ -std=c++11 C++/pld-photo.cpp -o C++/pld-photo `pkg-config --cflags --libs opencv`"
alias pld-photo-run="open-pld && ./C++/pld-photo"
alias pld-video-compile="open-pld && g++ -std=c++11 C++/pld-video.cpp -o C++/pld-video `pkg-config --cflags --libs opencv`"
alias pld-video-run="open-pld && ./C++/pld-video"

# $ source .bashrc
# ...