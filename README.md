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
<p align="center">
    <a href="https://axmol.dev" >
      <img src="https://axmol.dev/assets/img/logo.png" alt="Axmol" width="200"/>
    </a>
</p>

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

### Prerequisites

Download [Axmol](https://github.com/axmolengine/axmol), follow the steps in the [Dev Setup document](https://github.com/axmolengine/axmol/blob/dev/docs/DevSetup.md)

### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/hamstrous/CardGame.git
   ```
2. Make sure build will success
    Go to VS2026 -> Options -> Projects and Solutions -> Build and Run 
   ```sh
   Set maximum number of parallel projects builds to 2 (you can test to see how many build will suit your machine)
   ```
   After build complete you can set it back to the original value
   
   In projects folder -> open cmake -> AXGameEngineOptions.txt\
   Make sure AX_EXT_HINT is set to OFF
   ```sh
   set(AX_EXT_HINT OFF CACHE BOOL "The default extensions hint" FORCE)
   ```
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
