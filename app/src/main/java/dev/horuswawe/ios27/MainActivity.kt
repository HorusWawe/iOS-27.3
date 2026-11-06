package dev.horuswawe.ios27

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.animation.AnimatedContent
import androidx.compose.animation.core.animateFloatAsState
import androidx.compose.foundation.Image
import androidx.compose.foundation.background
import androidx.compose.foundation.clickable
import androidx.compose.foundation.gestures.detectVerticalDragGestures
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.grid.GridCells
import androidx.compose.foundation.lazy.grid.LazyVerticalGrid
import androidx.compose.foundation.lazy.grid.items
import androidx.compose.foundation.shape.CircleShape
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.*
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.alpha
import androidx.compose.ui.draw.blur
import androidx.compose.ui.draw.clip
import androidx.compose.ui.graphics.Brush
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.vector.ImageVector
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.input.pointer.pointerInput
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import java.time.LocalDateTime
import java.time.format.DateTimeFormatter

private val Ink = Color(0xFF090B14)
private val Glass = Color(0x42FFFFFF)
private val GlassBorder = Color(0x55FFFFFF)
private val White = Color(0xFFF8F8FC)

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        window.statusBarColor = android.graphics.Color.TRANSPARENT
        window.navigationBarColor = android.graphics.Color.TRANSPARENT
        window.decorView.systemUiVisibility =
            android.view.View.SYSTEM_UI_FLAG_LAYOUT_STABLE or
            android.view.View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN or
            android.view.View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
        setContent { IOS27App() }
    }
}

private data class AppEntry(val name: String, val icon: Int, val tint: Color)

private val apps = listOf(
    AppEntry("Настройки", R.drawable.ios_settings, Color(0xFF8E8E93)),
    AppEntry("Фото", R.drawable.ios_photos, Color(0xFFFF4B75)),
    AppEntry("Камера", R.drawable.ios_camera, Color(0xFF555B66)),
    AppEntry("Сообщения", R.drawable.ios_messages, Color(0xFF36C96B)),
    AppEntry("Safari", R.drawable.ios_safari, Color(0xFF36A5F5)),
    AppEntry("Музыка", R.drawable.ios_music, Color(0xFFFF3B55)),
    AppEntry("Телефон", R.drawable.ios_phone, Color(0xFF31C65B)),
    AppEntry("Календарь", R.drawable.ios_calendar, Color(0xFFF4F4F7)),
    AppEntry("Погода", R.drawable.ios_weather, Color(0xFF55B9F5)),
    AppEntry("Заметки", R.drawable.ios_notes, Color(0xFFFFD34E)),
    AppEntry("Часы", R.drawable.ios_clock, Color(0xFF20232D)),
    AppEntry("Файлы", R.drawable.ios_files, Color(0xFF4D9BFF))
)

@Composable
private fun IOS27App() {
    var unlocked by remember { mutableStateOf(false) }
    var activeApp by remember { mutableStateOf<String?>(null) }

    AnimatedContent(targetState = unlocked, label = "lock-home") { isUnlocked ->
        if (!isUnlocked) {
            LockScreen(onUnlock = { unlocked = true })
        } else if (activeApp != null) {
            AppSurface(name = activeApp!!, onBack = { activeApp = null })
        } else {
            HomeScreen(onOpen = { activeApp = it })
        }
    }
}

@Composable
private fun LockScreen(onUnlock: () -> Unit) {
    val now = remember { LocalDateTime.now() }
    var drag by remember { mutableFloatStateOf(0f) }
    val date = now.format(DateTimeFormatter.ofPattern("EEEE, d MMMM"))
        .replaceFirstChar { it.uppercase() }

    Box(
        Modifier.fillMaxSize().background(
            Brush.linearGradient(
                listOf(Color(0xFF182A52), Color(0xFF526D9A), Color(0xFFB6A9B5), Color(0xFF172039))
            )
        )
    ) {
        Box(Modifier.fillMaxSize().background(
            Brush.radialGradient(listOf(Color(0x557FCBFF), Color.Transparent), radius = 900f)
        ))
        Column(
            Modifier.fillMaxWidth().padding(top = 72.dp),
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            Text(date, color = White, fontSize = 19.sp, fontWeight = FontWeight.Medium)
            Spacer(Modifier.height(8.dp))
            Text(
                now.format(DateTimeFormatter.ofPattern("HH:mm")),
                color = White, fontSize = 88.sp, lineHeight = 92.sp,
                fontWeight = FontWeight.Light, letterSpacing = (-4).sp
            )
        }
        GlassPanel(Modifier.align(Alignment.TopCenter).padding(top = 250.dp).padding(horizontal = 20.dp)) {
            Row(Modifier.fillMaxWidth(), verticalAlignment = Alignment.CenterVertically) {
                Icon(R.drawable.ios_weather, null, tint = White, modifier = Modifier.size(30.dp))
                Spacer(Modifier.width(14.dp))
                Column(Modifier.weight(1f)) {
                    Text("Погода", color = White, fontWeight = FontWeight.SemiBold)
                    Text("Добавьте город в приложении", color = White.copy(alpha = .72f), fontSize = 12.sp)
                }
                Text("--°", color = White, fontSize = 30.sp, fontWeight = FontWeight.Light)
            }
        }
        Column(
            Modifier.align(Alignment.BottomCenter).fillMaxWidth().padding(bottom = 42.dp),
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            Row(Modifier.fillMaxWidth().padding(horizontal = 42.dp), horizontalArrangement = Arrangement.SpaceBetween) {
                LockShortcut(Icons.Default.FlashlightOn)
                LockShortcut(R.drawable.ios_camera)
            }
            Spacer(Modifier.height(28.dp))
            Box(
                Modifier.size(width = 150.dp, height = 5.dp).clip(CircleShape)
                    .background(Color.White.copy(alpha = .75f))
                    .pointerInput(Unit) {
                        detectVerticalDragGestures(
                            onVerticalDrag = { _, amount ->
                                drag += amount
                                if (drag < -34f) { drag = 0f; onUnlock() }
                            },
                            onDragEnd = { if (drag < -20f) onUnlock(); drag = 0f }
                        )
                    }
                    .clickable { onUnlock() }
            )
            Spacer(Modifier.height(10.dp))
            Text("Смахните вверх, чтобы открыть", color = White.copy(alpha = .85f), fontSize = 12.sp)
        }
        StatusPill(Modifier.align(Alignment.TopCenter).padding(top = 12.dp))
    }
}

@Composable
private fun HomeScreen(onOpen: (String) -> Unit) {
    Box(
        Modifier.fillMaxSize().background(
            Brush.linearGradient(listOf(Color(0xFF101D3A), Color(0xFF405D83), Color(0xFFB3A6AE), Color(0xFF26324E)))
        )
    ) {
        Column(Modifier.fillMaxSize().padding(top = 52.dp, start = 20.dp, end = 20.dp, bottom = 18.dp)) {
            Row(Modifier.fillMaxWidth(), horizontalArrangement = Arrangement.SpaceBetween, verticalAlignment = Alignment.CenterVertically) {
                Text("23:41", color = White, fontWeight = FontWeight.SemiBold, fontSize = 14.sp)
                StatusPill()
            }
            Spacer(Modifier.height(26.dp))
            GlassPanel(Modifier.fillMaxWidth()) {
                Column {
                    Text("ПОНЕДЕЛЬНИК", color = White.copy(alpha = .7f), fontSize = 11.sp, fontWeight = FontWeight.Bold, letterSpacing = 1.5.sp)
                    Text("Твой день.", color = White, fontSize = 27.sp, fontWeight = FontWeight.SemiBold)
                    Text("Всё важное рядом.", color = White.copy(alpha = .75f), fontSize = 13.sp)
                }
            }
            Spacer(Modifier.height(22.dp))
            LazyVerticalGrid(
                columns = GridCells.Fixed(4),
                modifier = Modifier.weight(1f),
                verticalArrangement = Arrangement.spacedBy(22.dp),
                horizontalArrangement = Arrangement.spacedBy(10.dp)
            ) {
                items(apps) { app ->
                    Column(horizontalAlignment = Alignment.CenterHorizontally, modifier = Modifier.clickable { onOpen(app.name) }) {
                        Box(
                            Modifier.size(61.dp).clip(RoundedCornerShape(18.dp))
                                .background(Brush.linearGradient(listOf(app.tint, app.tint.copy(alpha = .72f)))),
                            contentAlignment = Alignment.Center
                        ) {
                            Image(painter = painterResource(app.icon), contentDescription = app.name, modifier = Modifier.size(61.dp))
                        }
                        Spacer(Modifier.height(6.dp))
                        Text(app.name, color = White, fontSize = 11.sp, textAlign = TextAlign.Center, maxLines = 1)
                    }
                }
            }
            GlassPanel(Modifier.fillMaxWidth()) {
                Row(Modifier.fillMaxWidth(), horizontalArrangement = Arrangement.SpaceEvenly) {
                    listOf(apps[6], apps[3], apps[4], apps[5]).forEach { app ->
                        Box(Modifier.clickable { onOpen(app.name) }) {
                            Box(Modifier.size(55.dp).clip(RoundedCornerShape(17.dp)).background(app.tint), contentAlignment = Alignment.Center) {
                                Image(painter = painterResource(app.icon), contentDescription = app.name, modifier = Modifier.size(55.dp))
                            }
                        }
                    }
                }
            }
        }
        StatusPill(Modifier.align(Alignment.TopCenter).padding(top = 12.dp))
    }
}

@Composable
private fun AppSurface(name: String, onBack: () -> Unit) {
    val app = apps.firstOrNull { it.name == name }
    Column(
        Modifier.fillMaxSize().background(Color(0xFFF2F2F7)).padding(top = 52.dp, start = 20.dp, end = 20.dp)
    ) {
        Row(verticalAlignment = Alignment.CenterVertically) {
            Text("‹", color = Color(0xFF1685F8), fontSize = 36.sp, modifier = Modifier.clickable { onBack() })
            Spacer(Modifier.width(12.dp))
            Text(name, color = Color(0xFF17171B), fontSize = 21.sp, fontWeight = FontWeight.Bold)
        }
        Spacer(Modifier.height(24.dp))
        when (name) {
            "Настройки" -> {
                listOf("Авиарежим", "Wi-Fi", "Bluetooth", "Уведомления", "Основные", "Экран и яркость", "Обои", "Конфиденциальность").forEach {
                    Surface(color = Color.White, shape = RoundedCornerShape(14.dp), modifier = Modifier.fillMaxWidth().padding(vertical = 4.dp)) {
                        Row(Modifier.padding(16.dp), verticalAlignment = Alignment.CenterVertically) {
                            Text(it, color = Color(0xFF202027), modifier = Modifier.weight(1f))
                            Text("›", color = Color.Gray, fontSize = 24.sp)
                        }
                    }
                }
            }
            else -> {
                Surface(color = Color.White, shape = RoundedCornerShape(24.dp), modifier = Modifier.fillMaxWidth()) {
                    Column(Modifier.padding(22.dp)) {
                        if (app != null) Image(painter = painterResource(app.icon), contentDescription = app.name, modifier = Modifier.size(64.dp))
                        Spacer(Modifier.height(14.dp))
                        Text(name, fontSize = 24.sp, fontWeight = FontWeight.Bold, color = Color(0xFF17171B))
                        Text("Приложение оболочки iOS 27", color = Color.Gray, fontSize = 14.sp)
                        Spacer(Modifier.height(20.dp))
                        Text("Этот экран будет заменён полноценной реализацией приложения в следующих версиях.", color = Color.DarkGray)
                    }
                }
            }
        }
    }
}

@Composable
private fun GlassPanel(modifier: Modifier = Modifier, content: @Composable ColumnScope.() -> Unit) {
    Column(
        modifier.clip(RoundedCornerShape(26.dp))
            .background(Brush.linearGradient(listOf(Color(0x55FFFFFF), Color(0x22FFFFFF))))
            .padding(16.dp),
        content = content
    )
}

@Composable
private fun StatusPill(modifier: Modifier = Modifier) {
    Row(
        modifier.clip(CircleShape).background(Color(0xD9000000)).padding(horizontal = 13.dp, vertical = 7.dp),
        verticalAlignment = Alignment.CenterVertically
    ) {
        Box(Modifier.size(7.dp).clip(CircleShape).background(Color(0xFF65E58B)))
        Spacer(Modifier.width(7.dp))
        Text("iOS 27", color = Color.White, fontSize = 10.sp, fontWeight = FontWeight.SemiBold)
    }
}

@Composable
private fun LockShortcut(icon: ImageVector) {
    Box(Modifier.size(48.dp).clip(CircleShape).background(Color(0x55FFFFFF)), contentAlignment = Alignment.Center) {
        Icon(icon, null, tint = White, modifier = Modifier.size(22.dp))
    }
}
