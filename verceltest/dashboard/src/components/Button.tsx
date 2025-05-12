import { FC } from "react";

type ButtonProps = {
    variant?: "default" | "outline" | "destructive";
    onClick: () => void;
    children: React.ReactNode;
    type?: "button" | "submit" | "reset"; // Allow custom button types
};

const Button: FC<ButtonProps> = ({ variant = "default", onClick, children, type = "button" }) => {
    let buttonClass = "px-4 py-2 rounded-lg font-semibold transition duration-200";

    if (variant === "default") {
        buttonClass += " bg-blue-600 text-white hover:bg-blue-700";
    } else if (variant === "outline") {
        buttonClass += " border border-blue-600 text-blue-600 hover:bg-blue-100";
    } else if (variant === "destructive") {
        buttonClass += " bg-red-600 text-white hover:bg-red-700";
    }

    return (
        <button type={type} onClick={onClick} className={buttonClass}>
            {children}
        </button>
    );
};

export default Button;
