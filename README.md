<a id="readme-top"></a>
<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project
An Card game engine in Axmol c++, provide developers with a tool for quickly creating card game, also support multiplayer online

Of course, this is just the beginning for this project. So I'll be adding more functionaliies and improving user experience in the near future. You may also suggest changes by forking this repo and creating a pull request or opening an issue. 

<p align="right">(<a href="#readme-top">back to top</a>)</p>


### Built With

* [![Axmol][Axmol]][Axmol-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

### Prerequisites

Download [Axmol](https://github.com/axmolengine/axmol), follow the steps in the [Dev Setup document](https://github.com/axmolengine/axmol/blob/dev/docs/DevSetup.md)

### Installation

2. Clone the repo
   ```sh
   git clone https://github.com/hamstrous/CardGame.git
   ```
The steps below will specifically for building in VS2026 (ram of my development machine is 8GB)
3. Go to VS2026 -> Options -> Projects and Solutions -> Build and Run 
   ```sh
   Set maximum number of parallel projects builds to 2 (you can test to see how many build will suit your machine)
   ```
Or disable exernal tool to lighten up the build in the CMake file
This step will make sure that error C1060 compiler out of heap space does not occur
4. Run the solution

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- USAGE EXAMPLES -->
## Usage

I will try to write a detail documentation on how to use this engine in the future.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[Axmol]: https://axmol.dev/assets/img/logo.png
[Axmol-url]: https://axmol.dev
