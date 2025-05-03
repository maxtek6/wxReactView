# wxReactView

wxWidgets component for running React apps in WebView

## Why?

wxReactView was created for [a client](https://github.com/lariat-app),
which originally used React with Electron and the Node.js C++ plugin 
interface. While this was a workable solution, Electron was operating 
as a bloated middleware component between the React frontend and core 
C++ logic. The decision to start from scratch with wxWidgets had its 
own challenges, mostly related to loading local web content and message 
passing. This solution is meant to address these challenges.

Due to the flexibility and performance of the wxWidgets solution, our
client has given us permission to release this component as a separate 
project.

This is not meant to be a universal replacement for Electron, but for 
some use cases, it can provide a useful alternative. By extending the 
wxWidgets framework, wxReactView provides a solid connection between a
commonly used web technology and a feature-rich C++ desktop framework.

## Requirements

+ C++17
+ CMake 3.15 or newer
+ npm**

**optional, for building frontend components.

## Usage

The recommended toolchain for wxReactView is to use React + Vite.