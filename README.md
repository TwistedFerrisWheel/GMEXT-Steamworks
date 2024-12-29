# GMEXT-Steamworks
Repository for GameMaker's Steamworks Extension

This repository was created with the intent of presenting users with the latest version available of the extension (even previous to marketplace updates) and also provide a way for the community to contribute with bug fixes and feature implementation.

This extension will work on Windows, macOS and Linux.

## How to Contribute

SOURCE CODE: `source/steamworks_gml/extensions/steamworks/steamworks_cpp/`

Edit these files and then build.

## How to Build

Building requires you have each OS that you plan to build for.

In order to compile for Windows/macOS/Linux you are required to [download the Steam SDK v1.61](https://partner.steamgames.com/dashboard) and place it inside `source/Steamworks_sdk/`

> [!IMPORTANT]
> Version 2.0.0 is a major release with breaking changes:
>
> * You will need to make sure the SDK is updated to version 1.60
> * `steam_user_get_auth_session_ticket` can no longer be used for HTTP Web API purposes, this is now only for Game Server authentication (NOT HTTP, this is related to Steam Networking).
> * `steam_user_get_auth_ticket_for_web_api` must now be used for HTTP Web API, along with the identity parameter (not required but heavily recommended by Valve)

> [!IMPORTANT]
> Version 2.1.0 is requires Steamworks SDK version v1.61 and won't work correctly with previous versions of the SDK. If for any reason you need to keep using a previous SDK version please refrain from updating to the v2.1.0 version.

Once ready, follow these stems per OS.

### Windows

VISUAL STUDIO PROJECT: `source/steamworks_gml/extensions/steamworks/steamworks_windows/`

Download the latest visual studio (2022 at the time of writing) and hit build. This will create a steamworks_x64.dll file.

### MacOS

XCODE PROJECT: `source/steamworks_gml/extensions/steamworks/Steamworks_macos/`

I don't have a mac, please update this if you know. This will create a steamworks.dylib file.

### Linux

LINUX BUILD SCRIPTS: `source/steamworks_gml/extensions/steamworks/steamworks_linux/`

* CD to the `source` folder. 
* Give approprite read/write permissions to the child folders
* Run `./steamworks_gml/extensions/steamworks/steamworks_linux/build_linux64.sh`
* This will create a steamworks.so file.

### After Building

The building process automatically puts the files in a place that are usable in the provided example project. When ready to export to your project:
* Open the example project
* Navigate to the `Steamworks/Steamworks` extention
* Update / Add any functions needed (Click the hamburger menu to add a new function)
* When satisfied, go to `Tools > Create Local Package`, and add the `Steamworks` folder. Fill out the details however and click OK
* In your project, click `Tools > Import Local Package` and import the package you just created.

This should get your edited code into your project.

## Documentation

* Check [the documentation](../../wiki)

The online documentation is regularly updated to ensure it contains the most current information. For those who prefer a different format, we also offer a HTML version. This HTML is directly converted from the GitHub Wiki content, ensuring consistency, although it may follow slightly behind in updates.

We encourage users to refer primarily to the GitHub Wiki for the latest information and updates. The HTML version, included with the extension and within the demo project's data files, serves as a secondary, static reference.

Additionally, if you're contributing new features through PR (Pull Requests), we kindly ask that you also provide accompanying documentation for these features, to maintain the comprehensiveness and usefulness of our resources.

