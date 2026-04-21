#define UNICODE
#define _UNICODE

#include <windows.h>
#include <cairo.h>
#include <cairo-win32.h>

#define IDM_SAVE_PNG  1
#define IDM_SAVE_PDF  2

HWND g_hwnd;

void draw_scene(cairo_t *cr, int w, int h) {
    // 背景
    cairo_set_source_rgb(cr, 0.1, 0.1, 0.12);
    cairo_paint(cr);

    // 圆形
    cairo_set_source_rgb(cr, 0.2, 0.6, 1.0);
    cairo_arc(cr, w / 2, h / 2, 80, 0, 6.28);
    cairo_fill(cr);

    // 文字
    cairo_select_font_face(cr, "Segoe UI Emoji",
        CAIRO_FONT_SLANT_NORMAL,
        CAIRO_FONT_WEIGHT_BOLD);

    cairo_set_font_size(cr, 32);
    cairo_set_source_rgb(cr, 1, 1, 1);

    cairo_move_to(cr, 50, 80);
    cairo_show_text(cr, "Hello Cairo 🚀 中文测试 😊");

    cairo_move_to(cr, 50, 130);
    cairo_show_text(cr, "Graphics + Emoji + 中文排版");
}

void save_png() {
    RECT r;
    GetClientRect(g_hwnd, &r);

    int w = r.right;
    int h = r.bottom;

    cairo_surface_t *surface =
        cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);

    cairo_t *cr = cairo_create(surface);
    draw_scene(cr, w, h);

    cairo_surface_write_to_png(surface, "output.png");

    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    MessageBoxW(NULL, L"Saved PNG", L"OK", MB_OK);
}

void save_pdf() {
    cairo_surface_t *surface =
        cairo_pdf_surface_create("output.pdf", 800, 600);

    cairo_t *cr = cairo_create(surface);
    draw_scene(cr, 800, 600);

    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    MessageBoxW(NULL, L"Saved PDF", L"OK", MB_OK);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {

    case WM_CREATE: {
        HMENU menu = CreateMenu();
        HMENU file = CreatePopupMenu();

        AppendMenuW(file, MF_STRING, IDM_SAVE_PNG, L"Save PNG");
        AppendMenuW(file, MF_STRING, IDM_SAVE_PDF, L"Save PDF");
        AppendMenuW(file, MF_SEPARATOR, 0, NULL);
        AppendMenuW(file, MF_STRING, 3, L"Exit");

        AppendMenuW(menu, MF_POPUP, (UINT_PTR)file, L"File");
        SetMenu(hwnd, menu);
        break;
    }

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDM_SAVE_PNG:
            save_png();
            break;
        case IDM_SAVE_PDF:
            save_pdf();
            break;
        case 3:
            PostQuitMessage(0);
            break;
        }
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT r;
        GetClientRect(hwnd, &r);

        cairo_surface_t *surface =
            cairo_win32_surface_create(hdc);

        cairo_t *cr = cairo_create(surface);

        draw_scene(cr, r.right, r.bottom);

        cairo_destroy(cr);
        cairo_surface_destroy(surface);

        EndPaint(hwnd, &ps);
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }

    return 0;
}

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, PWSTR, int nCmdShow) {
    WNDCLASSW wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"CairoDemo";

    RegisterClassW(&wc);

    g_hwnd = CreateWindowW(
        L"CairoDemo", L"Cairo Windows Demo",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 900, 600,
        NULL, NULL, hInst, NULL
    );

    ShowWindow(g_hwnd, nCmdShow);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}
