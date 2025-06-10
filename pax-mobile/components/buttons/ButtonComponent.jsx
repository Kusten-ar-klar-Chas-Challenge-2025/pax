import { Pressable, StyleSheet, Text, View } from 'react-native'
import React from 'react'
import { useTheme } from '../../theme/ThemeContext'

const ButtonComponent = ({ title, onPress, style }) => {
        const { theme } = useTheme ();
        const styles = createStyles (theme);


  return (
    <Pressable
    onPress={onPress}
    style={({ pressed }) => [styles.button, pressed && styles.pressed]}>
     <Text style={styles.buttonText}>{title}</Text>
    </Pressable>
  )
}


export default ButtonComponent

const createStyles = (theme) =>
    StyleSheet.create({
    button: {
        margin: 10,
        padding: 20,
        backgroundColor: theme.card,
        borderRadius: 30,
        width: 200,
        textAlign: 'right',
        alignSelf: 'flex-end',
        marginRight: 20,
        borderColor: theme.accent,
        borderWidth: 2,
        fontFamily: 'NunitoSans'
      },
      pressed: {
        opacity: 0.5,
      },
      buttonText: {
        color: theme.textPrimary,
        textAlign: 'center',
        fontSize: 20,
      }
})