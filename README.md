# note-mbed

This example uses the Notecard and the [note-mbed][note-mbed] library with your MBED program.

First, you'll need to install the [mbed-cli v1][mbed-cli] onto your PC.  After this is installed and
the path/environment is set up correctly, the "mbed" command should work.

Download the [ARM GCC toolchain][arm-gcc] compiler.  Then configure MBED to use that toolchain by typing something like this, substituting your own path and making sure that
the path ends in the "bin" subfolder:
   ```
   mbed config -G GCC_ARM_PATH "C:\Program Files (x86)\GNU Tools ARM Embedded\8 2019-q3-update\bin"
   mbed toolchain -G GCC_ARM
   ```

Note that you can list your config at any time by using "mbed config --list".

Find the appropriate target board by using "mbed compile --supported".  In this example, I chose to use the
[ST Microelectronics Nucleo-L432KC][vendor-board] because of its ease of use and the fact that MBED support
[is available][mbed-target] for the board.  Instruct MBED that you are using this board by using the following:
   ```
   mbed target -G NUCLEO_L432KC
   ```

To validate that the entire toolchain is installed correctly, downloaded the "blinky" test by setting your
working directory to your development directory and using this command:
   ```
   mbed import git@github.com:ARMmbed/mbed-os-example-blinky.git
   ```

You'll compile it using this command, and the output ".bin" file will be in the build/<target>/<toolchain> directory.
   ```
   mbed compile
   ```

Plug your device into USB, and on Windows open "My Computer" you'll see the drive letter the MBED device was assigned
by Windows, such as this.  (If you don't see the drive after repeated tries, this may mean that your device may
not contain the MBED bootloader.  When shipped from the factory, the device has this bootloader installed.)
   ```
   NODE_L432KC (E:)
   ```

You may then install the bin onto the device by using a COPY command on Windows, or drag-and-drop on Mac.
   ```
   copy build\nucleo_l432kc\gcc_arm\*.bin E:
   ```

## Preparing your hardware for using this example

Connect your Nucleo-L432KC to the Notecarrier, as follows:
  - Connect the Notecarrier's RX to the Nucleo's D1/TX pin (USART1_TX PA_9)
  - Connect the Notecarrier's TX to the Nucleo's D0/RX pin (USART1_RX PA_10)
  - Connect the Notecarrier's SDA pin to the Nucleo's D4 pin (I2C1_SDA PB_7)
  - Connect the Notecarrier's SCL pin to the Nucleo's D5 pin (I2C1_SCL PB_6)

## Using this example

In your development directory where you're working on your MBED projects, create a new MBED test program by
using the command:
   ```
   mbed new my-test
   ```

Now, change your working directory into this "my-test" folder and clone this [note-mbed][note-mbed] repo into
a temporary folder somewhere on your PC.  Then, COPY the Notecard folder from that temporary folder into
your my-test folder, in its entirety.  After you do so, the folder hierarchy will be:

```
Your MBED projects folder
├──  my-test
  ├── mbed-os
  ├── Notecard
    ├── note-c
  ...
```

What this means is that
- Your project folder, "my-test", contains the mbed OS and all of its support files
- The Notecard subfolder contains everything in terms of support code to communicate with the Notecard
- You now just need a "main.cpp" to run your program

To get started with this example, look in the "examples" subfolder of your cloned copy of this repo.  Then,
copy one of the example's main.cpp into the main folder of your my-test project
   ```
   copy examples/01-using-Library/* <my-test path>
   ```

If you look at this main.cpp, you'll see that it implements the same functions as the
[note-arduino][note-arduino] library's analogous example.  The example has been tested using both Serial and I2C,
and so you can use either one as is your preference.

## Setting MBED OS
This repository requires MBED OS V5.15.  It is not compatible with MBED OS V6

To update the MBED version
```
cd mbed-os
mbed update mbed-os-5.15.8
```

## Compilation
Then, like the example above, you'll compile it and load it onto your board using this command
   ```
   mbed compile
   copy build\nucleo_l432kc\gcc_arm\*.bin E:
   ```
## Contributing


We love issues, fixes, and pull requests from everyone. By participating in this
project, you agree to abide by the Blues Inc [code of conduct].

For details on contributions we accept and the process for contributing, see our
[contribution guide](CONTRIBUTING.md).

## More Information

For Notecard SDKs and Libraries, see:

* [note-c][note-c] for Standard C support
* [note-go][note-go] for Go
* [note-python][note-python] for Python
* [note-arduino][note-arduino] for Arduino

## To learn more about Blues Wireless, the Notecard and Notehub, see:

* [blues.com](https://blues.io)
* [notehub.io][Notehub]
* [wireless.dev](https://wireless.dev)

## License

Copyright (c) 2019 Blues Inc. Released under the MIT license. See
[LICENSE](LICENSE) for details.

[blues]: https://blues.com
[notehub]: https://notehub.io
[note-go]: https://github.com/blues/note-go
[note-python]: https://github.com/blues/note-python
[archive]: https://github.com/blues/note-arduino/archive/master.zip
[code of conduct]: https://blues.github.io/opensource/code-of-conduct
[Notehub]: https://notehub.io

[mbed-cli]: https://os.mbed.com/docs/mbed-os/v6.15/build-tools/install-and-set-up.html
[arm-gcc]: https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads
[vendor-board]: https://www.st.com/en/evaluation-tools/nucleo-l432kc.html
[mbed-target]: https://os.mbed.com/platforms/ST-Nucleo-L432KC/
[note-c]: https://github.com/blues/note-c
[note-mbed]: https://github.com/blues/note-mbed
[note-arduino]: https://github.com/blues/note-arduino
[board]: https://www.st.com/en/evaluation-tools/nucleo-l031k6.html
[reference-manual]: https://www.st.com/resource/en/user_manual/dm00231744.pdf
[ide]: https://www.st.com/en/development-tools/stm32cubeide.html
