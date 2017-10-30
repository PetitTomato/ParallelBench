################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../image.c \
../main.c \
../parallel_app.c \
../ransac.c 

OBJS += \
./image.o \
./main.o \
./parallel_app.o \
./ransac.o 

C_DEPS += \
./image.d \
./main.d \
./parallel_app.d \
./ransac.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -std=c99 -I../camera_stub -O3 -pg -Wall -c -fmessage-length=0 -m32 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


