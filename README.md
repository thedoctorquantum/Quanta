# Quanta

A C++, cross platform desktop game engine with a focus on data oriented and data driven design, with support for primarily 3D applications, and with planned support for 2D rendering. 

### **The Planned Features**

* Support for modern Graphics APIs such as Vulkan, Direct3D12 and Metal
* A flexible, fast and extensible 3D renderer.
* A fast set of voxel and particle rendering and physics systems.
* A data oriented ECS (Entity Component System) scene archetecture.
* Data driven configuration via Json and Xml
* A data driven UI system controlled via a markup language
* Extensive scripting via AngelScript

---

## How do I build it?

### **Prerequisites**

* Git - https://git-scm.com/
* CMake - https://cmake.org/  
* GCC - https://gcc.gnu.org/
* LunarG Vulkan SDK - https://www.lunarg.com/vulkan-sdk/

### **My Environment**

These items aren't necessary, but they are recommended over alternatives as the project is built with them.

* Visual Studio Code - https://code.visualstudio.com/

### **Build Steps**

**1. Clone the repository**

* `git clone --recursive https://github.com/thedoctorquantum/Quanta.git`.
* `git submodule update --init`

**2. Configure CMake**

* `cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=OFF -DGLFW_BUILD_DOCS=OFF -S . -B Build`

**3. Build Sandbox**

At the moment the main way to build Quanta is to build the sub-project "Sandbox", as you get an actively developed example along with the library itself.

* `cd Build/Sandbox && make && ./Sandbox`

## Licensing

You can use Quanta wherever and for whatever you like. It's MIT.

