# Installation {#installation}

The following guides are based on some shared assumptions:

- You have `git` installed
- You have basic skills of operating file system & textual configs

## Downloading and proper folders structure {#download-and-folder-structure}

1. Create new folder with any name, that will not contain non-latin symbols, and spaces/other special symbols too

2. Go to this folder, open command prompt, and run this command

   ```sh
   git clone --recursive https://github.com/fonline-roleplay/fo4rp Server
   ```

   `git clone` command will create local copy of repository in `Server` folder, and then download it's up-to-date commit

3. Go to `Server` folder, copy `tnfFonlineServer.cfg` and name it `Server.cfg`

4. Go back to the folder that you've created, and create `Client` folder

5. Download this [client updater tool](https://cdn.discordapp.com/attachments/1319029614776946808/1500537393776693531/TLJ_updater.exe?ex=6aa62919&is=6aa4d799&hm=06de2e47f526c8de853aaee8a9ad26f5b68ad1c58f9ad70454ee95ff0489a6dd&) and lay it down to `Client` folder, run it and wait for `Done. Press Enter to exit.`, press "Enter" and close command prompt.

6. Go to `Server` folder and run `Server64.exe`, then press `Start Server`, after that you should be able to connect on default address `localhost:4000` from client

## Running the Mapper {#running-mapper}

1. Go to `Server/Tools/Mapper/` folder

2. (Optional) Find and open `Mapper.cfg` in any suitable text editor, specify both client and server paths, if default doesn't work

3. Run `Mapper64.exe`