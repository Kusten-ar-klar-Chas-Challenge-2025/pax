import { baseTheme } from "./colors";

export const buildCustomTheme = (baseTheme, isDark) => ({
    dark: isDark,
    colors: {
      background: baseTheme.background,
      card: baseTheme.card,
      textLight: baseTheme.textLight,
      textDark: baseTheme.textDark,
      textPrimary: baseTheme.textPrimary,
      textSecondary: baseTheme.textSecondary,
      primary: baseTheme.primary,
      secondary: baseTheme.secondary,
      accent: baseTheme.accent,
      tabBar: baseTheme.tabBackground,
      buttonColor: baseTheme.buttonColor,
    },
    fonts: {
      regular: { fontFamily: "NunitoSans, Sans-serif", fontWeight: "normal" },
      medium: { fontFamily: "NunitoSans, Sans-serif", fontWeight: "500" },
    },
    fontFamily: "BadScript-Regular", 
    fontFamily: 'NunitoSans.ttf',
    fontFamily: 'Nunito',
    fontFamily: 'NunitoSans',
    fontFamily: "Nunito.ttf"
  });
