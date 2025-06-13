struct spin_lock_t {
    enum	{
        SPIN_LOCK_FREE	= 0,
        SPIN_LOCK_TAKEN	= -1
    };
    volatile int _lock;

    inline void	lock () noexcept {
        for (;;) {
            // Optimistically assume the lock is free on the first try
            if (!cmpxchg(_lock, SPIN_LOCK_TAKEN, SPIN_LOCK_FREE)) {
                break;
            }

            // Wait for lock to be released without generating cache misses
            uint8_t wait = 1;
            while (SPIN_LOCK_FREE != load(&_lock)) {
                wait *= 2; // exponential backoff if can't get lock
#if (defined(__PROSPERO__) || defined(_GAMING_XBOX))
                // hardware wait on address when value changes
                _mm_monitorx((void *)&_lock, 0, 0);
                _mm_waitx(0, 0, wait);
#else
                for (u8 i = 0; i < wait; i++)
                    __mm_pause();
#endif                  
                wait += (wait ? 0 : 1);
            }
        }
    }
  
    inline void	unlock	() noexcept {
        _lock = SPIN_LOCK_FREE;
    }
  
    inline bool	trylock	() noexcept {
        // First do a relaxed load to check if lock is free in order to prevent
        // cache misses if someone does while(!try_lock())
        if ((SPIN_LOCK_FREE == load(&_lock))
            && (SPIN_LOCK_FREE == cmpxchg(_lock, SPIN_LOCK_TAKEN, SPIN_LOCK_FREE)) ) {
            return (true);
        } else {
            return (false);
        }
    }
  
    // just debug check
    inline	bool locked	() const noexcept {
        return (SPIN_LOCK_FREE != _lock);
    }

    inline spin_lock_t(pcstr = 0) noexcept : _lock(SPIN_LOCK_FREE) {}
    inline ~spin_lock_t() noexcept {}
};
