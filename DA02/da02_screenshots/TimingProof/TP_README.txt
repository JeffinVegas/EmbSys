Brief Explanations about each photo:

Each photo shows proof of the correct period. Tasks 1, 3, 4, and 5 are supposed to have a period of 0.5 seconds, which means the light should toggle on/off every 0.25 seconds.

ASM_T1: A breakpoint was placed after the 0.25 second delay, right before the LED light toggles. The stop watch shows approximately 249.99K microseconds, which is about 0.25 seconds.

ASM_T2: A breakpoint was placed after the 1 second delay, right before the LED light turns back off. The stop watch shows approximately 999.68K microseconds, which is about 1 second. The code was modified as if the button was immediately pushed.

ASM_T3: A breakpoint was placed after the 0.25 second delay using TCNT0, right before the LED light toggles. The stop watch shows approximately 248.96K microseconds, which is about 0.25 seconds.

ASM_T4: A breakpoint was placed after the 0.25 second delay in the TIM0_OVF interrupt, right before the LED light toggles. The stop watch shows approximately 251.91K microseconds, which is about 0.25 seconds.

ASM_T5: There are two photos.
a) (If the button is not pushed) The breakpoint was placed after the 0.25 second delay, right before the LED light toggles. The stop watch shows approximately 248.96K microseconds, which is about 0.25 seconds.
b) (If the button is pushed) The stop watch continued from the first Task 5 photo. The second breakpoint was placed after the 1 second delay, right before the LED light toggles. The stop watch shows approximately 1245K microseconds, which is about 1 second delay after the LED toggled. The video shows that the LED will toggle in between rising/falling edge.

C_T1: A breakpoint was placed after the 0.25 second delay, right before the LED light toggles off. The stop watch shows approximately 250K microseconds, which is about 0.25 seconds.

C_T2: A breakpoint was placed after the 1 second delay, right before the LED light turns back off. The stop watch shows approximately 1000K microseconds, which is about 1 second. The code was modified as if the button was immediately pushed.

C_T3: A breakpoint was placed after the 0.25 second delay, right before the LED light toggles. The stop watch shows approximately 250.42K microseconds, which is about 0.25 seconds.

C_T4: A breakpoint was placed after the 0.25 second delay in the TIM0_OVF_vect interrupt, right before the LED light toggles. The stop watch shows approximately 250.17K microseconds, which is about 0.25 seconds.

C_T5: A breakpoint was placed after the 0.25 second delay, right before the LED light toggles off. The stop watch shows approximately 1250K microseconds, which is about 1.25 seconds (250K from the while toggle and 1000K microseconds from the INT0_vect interrupt). The video shows that the LED will toggle in between rising/falling edge.