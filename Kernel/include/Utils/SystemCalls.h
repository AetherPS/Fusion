#pragma once

caddr_t sys_mmap(thread* td, caddr_t addr, size_t len, int prot, int flags, int fd, off_t pos);
int sys_munmap(thread* td, caddr_t addr, size_t len);