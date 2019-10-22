# sooScreenShare (super optimal omniusable screen share)
A new tool for screen sharing over network.

#### The name is the aim! 
We want to develop software which has the ability of transporting the image of a whole screen over the network. The intended use case is to use another PC as desktop extension. For instance, your old notebook as screen extension for your new notebook. We want to support various operating systems.

#### How it works
##### Up to now only Linux (and windows with the windows branch) is supported
The machine which has to share a screen runs the server application. You need to create a virtual screen on your own.
The machine which has to display the shared screen runs the client application. You should connect these machines with an appropriate network connection. In development a gigabit connection is used. Configure the two applications according to your needs.

#### Disclamier: We are in an early development stage. Please use this code only if you know what you are doing!

### If you want to contribute:
We would appreciate your help, especially on the following topics: 
- documentation or ideas on how to create a virtual screen under linux, windows and osx
- multiplatform screenshot backend
- new compression backends (CPU friendly and/or network friendly) (We will implement a differential image algorithm as precompression soon).
