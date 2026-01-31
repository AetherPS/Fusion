#include "common.h"
#include "log.h"

static inline void kputc(char c, char** buf, int* len, int maxlen)
{
	if (*len < maxlen - 1)
	{
		**buf = c;
		(*buf)++;
		(*len)++;
	}
}

static void kputs(const char* s, char** buf, int* len, int maxlen)
{
	if (!s) s = "(null)";
	while (*s)
	{
		kputc(*s++, buf, len, maxlen);
	}
}

static void kputnum(unsigned long long num, int base, int is_signed, char** buf, int* len, int maxlen)
{
	char digits[32];
	int i = 0;
	int is_neg = 0;

	if (is_signed && (long long)num < 0)
	{
		is_neg = 1;
		num = -(long long)num;
	}

	if (num == 0)
	{
		digits[i++] = '0';
	}
	else
	{
		while (num > 0)
		{
			int digit = num % base;
			digits[i++] = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
			num /= base;
		}
	}

	if (is_neg)
	{
		kputc('-', buf, len, maxlen);
	}

	while (i > 0)
	{
		kputc(digits[--i], buf, len, maxlen);
	}
}

int kvsnprintf(char* str, int size, const char* fmt, __builtin_va_list ap)
{
	char* buf = str;
	int len = 0;
	const char* p = fmt;

	while (*p && len < size - 1)
	{
		if (*p != '%')
		{
			kputc(*p++, &buf, &len, size);
			continue;
		}

		p++; // skip '%'

		// Handle field width
		int width = 0;

		if (*p == '*')
		{
			width = __builtin_va_arg(ap, int);
			p++;
		}
		else
		{
			while (*p >= '0' && *p <= '9')
			{
				width = width * 10 + (*p - '0');
				p++;
			}
		}

		// Handle format specifiers
		switch (*p)
		{
		case 's': {
			const char* s = __builtin_va_arg(ap, const char*);
			if (!s) s = "(null)";

			int str_len = 0;
			const char* tmp = s;
			while (*tmp++) str_len++;

			// Apply width (right-aligned by default)
			int padding = width - str_len;
			if (padding > 0)
			{
				for (int i = 0; i < padding; i++)
				{
					kputc(' ', &buf, &len, size);
				}
			}

			kputs(s, &buf, &len, size);
			break;
		}
		case 'd':
		case 'i': {
			int val = __builtin_va_arg(ap, int);
			kputnum(val, 10, 1, &buf, &len, size);
			break;
		}
		case 'u': {
			unsigned int val = __builtin_va_arg(ap, unsigned int);
			kputnum(val, 10, 0, &buf, &len, size);
			break;
		}
		case 'x': {
			unsigned int val = __builtin_va_arg(ap, unsigned int);
			kputnum(val, 16, 0, &buf, &len, size);
			break;
		}
		case 'p': {
			void* ptr = __builtin_va_arg(ap, void*);
			kputs("0x", &buf, &len, size);
			kputnum((unsigned long long)ptr, 16, 0, &buf, &len, size);
			break;
		}
		case 'l': {
			p++; // skip 'l'
			if (*p == 'l')
			{
				p++; // skip second 'l' for 'lld'
				if (*p == 'd')
				{
					long long val = __builtin_va_arg(ap, long long);
					kputnum(val, 10, 1, &buf, &len, size);
				}
				else if (*p == 'u')
				{
					unsigned long long val = __builtin_va_arg(ap, unsigned long long);
					kputnum(val, 10, 0, &buf, &len, size);
				}
				else if (*p == 'x')
				{
					unsigned long long val = __builtin_va_arg(ap, unsigned long long);
					kputnum(val, 16, 0, &buf, &len, size);
				}
			}
			else if (*p == 'd')
			{
				long val = __builtin_va_arg(ap, long);
				kputnum(val, 10, 1, &buf, &len, size);
			}
			else if (*p == 'u')
			{
				unsigned long val = __builtin_va_arg(ap, unsigned long);
				kputnum(val, 10, 0, &buf, &len, size);
			}
			else if (*p == 'x')
			{
				unsigned long val = __builtin_va_arg(ap, unsigned long);
				kputnum(val, 16, 0, &buf, &len, size);
			}
			break;
		}
		case '%': {
			kputc('%', &buf, &len, size);
			break;
		}
		case 'c': {
			char c = (char)__builtin_va_arg(ap, int);
			kputc(c, &buf, &len, size);
			break;
		}
		default:
			kputc('%', &buf, &len, size);
			kputc(*p, &buf, &len, size);
			break;
		}
		p++;
	}

	*buf = '\0';
	return len;
}

void klog(const char* fmt, ...)
{
	char buffer[512];
	__builtin_va_list args;
	__builtin_va_start(args, fmt);
	kvsnprintf(buffer, sizeof(buffer), fmt, args);
	__builtin_va_end(args);

	syscall(601, 7, buffer, 0);
}