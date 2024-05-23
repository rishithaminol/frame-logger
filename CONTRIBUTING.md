# How to contribute

Volunteer developers are welcome to help this project. It will be more helpfull designing a reliable system.

## Testing

Currently the testing mechanism is driven by cmake `ctest`. In future more and more integrated testing mechanisms will be covered. Operating system perspective, FreeBSD and RHEL systems (Fedora, Almalinux) are covered.

  * When you do the development, try to implement unit-tests for your changes if possible.
  * Run `ctest` after you have completed your *.c file developments. Which will be useful before make a `git push`.
  * New ways or mechanisms of testing are welcomed.

## Submitting changes



## Coding conventions

Start reading our code and you'll get the hang of it. We optimize for readability:

  * [Linux kernel coding style](https://www.kernel.org/doc/html/v4.10/process/coding-style.html) will be used as the standard C programming coding style. If there are Python/Shell Scripting scripts, more than 80% of practice absorbed from this documentation.
  * When you feel success about your development, generate doxygen documentation and look whether there is function calls similar to this image. Definitely this kind of situation will lead the program into caose. ![](images/callback_loop_danger.png)
