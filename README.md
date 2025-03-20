### Maven !

Welcome to maven, a programming language that:
1) Emphasizes the ability to do anything whether low or high level
2) Opt-in safety, maven won't be aggressive against unsafe actions unless you ask it to!
3) Ensures readability and a smooth learning curve

Currently maven is **EXPERIMENTAL** and not recommended for production usage,

Feel free to experiment with maven, leave a star if you think it's cool and make an issue if you break it!

You can check out the docs here: <insert docs link when live>

### Download
Check releases (for macos)

### Building from source
**Requirements:**
- llvm

Simply:
```bash
git clone https://github.com/Mavenista/Maven/
cd Maven
bash assemble.sh <your operating system>
```
**<sup><sub>(Assemble.sh may need sudo depending on your OS)</sup></sub>**

Now there will be a file called `Maven-2025b-<your operating system>.tar.gz` in `Release/2025b/`

### Installation
Once you have compiled and bundled the .tar.gz whether from **Download** or **Building from source** ,

```bash
mkdir /usr/local/bin/maven
tar -xvzf <path to Maven-2025b-<your operating system>.tar.gz> -C /usr/local/bin/maven
```
lastly, you will need to add the folder to PATH for macos users this is navigating to `~/.zshrc` (or your preferred shell) and adding the line:
```bash
export PATH="/usr/local/bin/maven:$PATH"
```

Verify the installation with `maven version`:
`Maven: Anthology no. 2025b (Arcadia)`

### Project Structure
```
maven/ - the project management component
mavenc/ - the compiler management component
polaroid/ - the llvm codegen component
std/ - the maven std
```

