# T7MTEnhancements
[![Donate](https://img.shields.io/badge/Donate-PayPal-yellowgreen.svg)](https://www.paypal.me/scobalula) [![Releases](https://img.shields.io/github/downloads/Scobalula/T7MTEnhancements/total.svg)](https://github.com/Scobalula/T7MTEnhancements/) [![Discord](https://img.shields.io/badge/chat-Discord-blue.svg)](https://discord.gg/RyqyThu)

This is a patch for Linker (and possibly other tools within Black Ops III's Mod Tools in the future) that adds some enhancements to Linker:

* Overrides internal routines to allow linker to take the hash value from the hash (i.e. if provided with `var_7ec446a0` then the hash would be `0x7ec446a0`). It currently looks for function_, hash_, var_, and namespace_. This makes it very useful to call functions from other scripts (for example if you needed to call a function from a map specific script for a mod), override scripts, etc. without knowing their original names.
* Provides a table that allows you to dictate a hash value for a given string. This is useful for hashes you can't figure out, but want to provide a name for while maintaining the hash.

In the future other enhancements will be provided for both linker, and other tools such as APE, Radiant, etc.

# Installing

To install simply download the latest version from [Releases](https://github.com/Scobalula/T7MTEnhancements/releases) and copy the 2 DLL files into `<Black Ops 3 Tools Folder>\bin`. This can be installed alongside DTZxPorter/SE2Dev/Nukem's L3akMod without any issues.

To uninstall, simply remove `PhilLibX.T7MTEnhancements` from your bin folder.

Please report any issues that occur.

# Using the Substitute Table/s

As mentioned above, the tool can load csv tables with strings and hash overrides to allow you to provide a name for a given string, even if the name given is not the original. This can be useful if you're distributing scripts that rely on hashed names while providing a meaningful name in its place.

To do this simple create or edit a csv file in `<Black Ops 3 Tools Folder>\bin\HashTables` (it will load all CSV files in this folder) and add a name and a hash value, like so:

```bgb_button_pressed,0x10c37787```

Then you can save and recompile, and easily provide this file to others to use with your scripts so long as they have this patch.

# Credits:

* Nukem9 - [Detours](https://github.com/Nukem9/detours)

# License/Disclaimer

Please read the License.txt file in the download before using this patch.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

If you find use in this, it would be great if you would credit me, or if you want to support me even more, [Donate](https://www.paypal.me/scobalula).