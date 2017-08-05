## Def-Diags
Simpe cross-platform (OSX, Windows, Linux) native open and save dialogs for Defold.

Uses zlib licensed [mlabbe/nativefiledialog](https://github.com/mlabbe/nativefiledialog) with some modifications (see `diags/src/mlabbe-nativefiledialog/changes.txt`).

### Usage
```Lua
-- Open file dialog
print("diags.open:", diags.open())

-- Save file dialog
print("diags.save:", diags.save())

-- Open multiple files
local r, t = diags.open_multiple()
print("diags.open_multiple:", r)
pprint(t)

-- Open folder
print("diags.open_folder:", diags.open_folder())

```

All functions take a default path as argument. `diags.open()`, `diags.save()` and `diags.open_multiple()` also takes a file extension filter as the first argument. Example:
```Lua
diags.open("png,jpg;psd", "/tmp/")
```
For more information see: https://github.com/mlabbe/nativefiledialog#file-filter-syntax
