import React from "react";
import {
  View,
  Text,
  StyleSheet,
  ScrollView,
  StatusBar,
  Platform,
  Pressable,
  Image,
  Dimensions,
} from "react-native";
import { SafeAreaView, SafeAreaProvider } from "react-native-safe-area-context";
import { MaterialCommunityIcons } from "@expo/vector-icons";
import { useTheme } from "../theme/ThemeContext";
import CardComponent from "../components/CardComponent";
import RoomCard from "../components/RoomCard";
import WelcomeCard from "../components/WelcomeCard";
import { useUser } from "../context/UserContext";

const { width, height } = Dimensions.get("window");

const HomeScreen = () => {
  const { theme, isDark, toggleTheme } = useTheme();
  const styles = createStyles(theme);
  const {user} = useUser();

  return (
    <SafeAreaProvider>
      <ScrollView style={styles.ScrollView}>
        <SafeAreaView style={styles.safeArea}>
          <View style={styles.header}>
            <StatusBar
              barStyle={isDark ? "light-content" : "dark-content"}
              backgroundColor={theme.headerBackground}
            />
            <Pressable
              onPress={toggleTheme}
              accessible={true}
              accessibilityRole="button"
              accessibilityLabel={
                isDark ? "Byt till ljust tema" : "Byt till mörkt tema"
              }
              style={styles.toggleButton}
            >
              <MaterialCommunityIcons
                name={isDark ? "white-balance-sunny" : "weather-night"}
                size={30}
                color={theme.iconSwitchmode}
              />
            </Pressable>
            <Image
              source={require("../assets/logo/pax_logo-text-10.png")}
              style={styles.logo}
              resizeMode="contain"
            />
          </View>
          <WelcomeCard />
          <RoomCard />
          <View style={styles.container}>
            <Image
              source={require("../assets/illustrationer/illustrationer_pax-27.svg")}
              style={styles.image}
              resizeMode="contain"
            />
          </View>
        </SafeAreaView>
      </ScrollView>
    </SafeAreaProvider>
  );
};

const createStyles = (theme) =>
  StyleSheet.create({
    safeArea: {
      flex: 1,
      backgroundColor: theme.background,
    },

    ScrollView: {
      backgroundColor: theme.background,
    },

    header: {
      flexDirection: "row",
      justifyContent: "center",
      alignItems: "center",
      paddingVertical: 16,
      position: "relative",
    },
    container: {
      alignItems: "center",
      marginTop: 100,
    },
    logo: {
      width: width * 0.4,
      height: 50,
    },
    image: {
      width: width * 1.2,
      height: height * 0.3,
      opacity: 0.75,
    },
    toggleButton: {
      position: "absolute",
      top: Platform.OS === "ios" ? 0 : 10,
      left: 20,
      backgroundColor: "#E3FAC825",
      borderRadius: 25,
      padding: 5,
      zIndex: 1,
    },
    text: {
      fontSize: 18,
      color: theme.text,
    },
  });

export default HomeScreen;
