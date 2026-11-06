# iOS 27.3 for TECNO KL5n

An Android-native iOS-inspired system shell, built for Android 14 (API 34) on ARM64 devices.

## Project status

The repository starts with the native Android shell and a Magisk systemless boot-animation module. This is a staged implementation: a launcher app cannot replace Android Keyguard or SystemUI by itself. Those integrations will be developed as separate privileged/system components after the base shell is stable.

## First milestone

- Native Kotlin + Jetpack Compose launcher shell
- iOS-inspired lock-screen preview and home screen
- Dock, app grid, status indicators, Dynamic Island-style activity surface
- Magisk module for `/product/media/bootanimation.zip`
- GitHub Actions build artifacts: APK and Magisk ZIP

## Device target

- TECNO KL5n
- Android 14 / API 34
- ARM64
- MediaTek MT6768

## Build

Open the project in Android Studio or run `./gradlew assembleDebug`.
